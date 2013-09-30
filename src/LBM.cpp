/* Copyright (C) 2013  Dominik Ernst

   This file is part of Rabenstein.

   Feldrand is free software: you can redistribute it and/or modify it under the
   terms of the GNU Affero General Public License as published by the Free
   Software Foundation, either version 3 of the License, or (at your option) any
   later version.

   This program is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
   FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more
   details.

   You should have received a copy of the GNU Affero General Public License along
   with this program.  If not, see <http://www.gnu.org/licenses/>. */

#include "LBM.hpp"
#include <sys/types.h>
#include <sys/time.h>

using namespace std;

namespace Rabenstein {

namespace {

const float fluid[] = {
    1.0f/36.0f, 1.0f/9.0f, 1.0f/36.0f,
    1.0f/9.0f,  4.0f/9.0f, 1.0f/9.0f,
    1.0f/36.0f, 1.0f/9.0f, 1.0f/36.0f
};

const float source[] = {
    1.4f/36.0f, 1.4f/9.0f, 1.4f/36.0f,
    1.4f/9.0f,  3.0f/9.0f, 1.4f/9.0f,
    1.4f/36.0f, 1.4f/9.0f, 1.4f/36.0f
};

const float drain[] = {
    0.6f/36.0f, 0.6f/9.0f, 0.6f/36.0f,
    0.6f/9.0f,   5.0f/9.0f, 0.6f/9.0f,
    0.6f/36.0f, 0.6f/9.0f, 0.6f/36.0f
};
}

LBM::LBM():
    gridWidth(0),
    gridHeight(0),
    getVelocityKernel(NULL),
    getDensityKernel(NULL),
    simulationStepKernel(NULL) {}

LBM::LBM(size_t grid_width, size_t grid_height)
    : gridWidth(grid_width),
      gridHeight(grid_height),
      getVelocityKernel(NULL),
      getDensityKernel(NULL),
      simulationStepKernel(NULL),
      vel(grid_width, grid_height),
      dens(grid_width, grid_height)
{
    // OpenCL initialization needs to be doen from the same thread that calls
    // the kernels later
    if( !OpenCLHelper::isOpenCLAvailable() ) {
        cout << "opencl library not found" << endl;
        exit(-1);
    }

    cl = new OpenCLHelper(0);
    getVelocityKernel = cl->buildKernel("../src/getVelocity.cl",
                                        "getVelocity");
    getDensityKernel = cl->buildKernel("../src/getDensity.cl",
                                       "getDensity");
    simulationStepKernel = cl->buildKernel("../src/simulationStep.cl",
                                           "simulationStep");

    for( size_t i = 0; i < 9; i++) {
        src[i] = cl->arrayFloat(gridWidth*gridHeight);
        src[i]->createOnHost();
        dst[i] = cl->arrayFloat(gridWidth*gridHeight);
        dst[i]->createOnHost();
    }
    flag_field = cl->arrayInt(gridWidth*gridHeight);
    flag_field->createOnHost();

    global_size[0] = OpenCLHelper::roundUp(64, gridWidth);
    global_size[1] = OpenCLHelper::roundUp(64, gridHeight);
    local_size[0] = 16;
    local_size[1] = 16;

    do_clear();
}

LBM::~LBM() {
    delete getVelocityKernel;
    delete getDensityKernel;
    delete simulationStepKernel;
    delete cl;
}

double dtime() {
    double tseconds = 0;
    struct timeval t;
    gettimeofday( &t, NULL);
    tseconds = (double) t.tv_sec + (double) t.tv_usec*1.0e-6;
    return tseconds;
}

void LBM::one_iteration() {
    simulationStepKernel->input( (int) gridWidth);
    simulationStepKernel->input( (int) gridHeight);
    for(size_t i = 0; i < 9; i++) {
        simulationStepKernel->input( src[i] );
    }
    for(size_t i = 0; i < 9; i++) {
        simulationStepKernel->output( dst[i] );
    }
    simulationStepKernel->input(flag_field);

    simulationStepKernel->run(2, global_size, local_size);
    for( size_t i = 0; i < 9; i++) {
        std::swap(src[i], dst[i]);
    }
    simulationStepKernel->finishPending();

}
void LBM::setFields(const size_t ix, const size_t iy,
                        const float* val, const int type) {
    for( size_t i = 0; i < 9; i++) {
        (*(src[i]))[iy*gridWidth + ix] = val[i];
        (*(dst[i]))[iy*gridWidth + ix] = val[i];
    }
    (*flag_field)[iy*gridWidth + ix] = type;
}

void LBM::do_clear() {
    for(size_t iy = 0; iy < gridHeight; ++iy) {
        for(size_t ix = 0; ix < gridWidth; ++ix) {
            setFields(ix, iy, fluid, (int) cell_t::FLUID);
        }
    }

    for(size_t iy = 0; iy < gridHeight; ++iy) {
        setFields(0,           iy, source,  (int) cell_t::SOURCE);
        setFields(gridWidth-1, iy, drain, (int) cell_t::SOURCE);
    }

    for(size_t ix = 0; ix < gridWidth; ++ix) {
        setFields(ix, 0,            fluid, (int) cell_t::NO_SLIP);
        setFields(ix, gridHeight-1, fluid, (int) cell_t::NO_SLIP);
    }

    for(size_t i = 0; i < gridHeight/4; ++i) {
        setFields(gridWidth/5, i, fluid, (int) cell_t::NO_SLIP);
        setFields(gridWidth/5, gridHeight-i-1, fluid, (int) cell_t::NO_SLIP);
    }

    for(size_t i = 0; i < 9;i++) {
        dst[i]->copyToDevice();
        src[i]->copyToDevice();
    }


    flag_field->copyToDevice();
}

auto LBM::getVelocity() -> Grid<Vec2D<float>>* {

    if( getVelocityKernel == NULL) return NULL;

    for( size_t i = 0; i < 9; i++) {
        getVelocityKernel->input( src[i] );
    }

    getVelocityKernel->output( vel.x() * vel.y() * 2, reinterpret_cast<float*>(vel.data()));
    getVelocityKernel->input( (int) gridWidth );
    getVelocityKernel->input( (int) gridHeight );

    getVelocityKernel->run(2, global_size, local_size );

    Grid<Vec2D<float>>* g = new Grid<Vec2D<float>>(gridWidth, gridHeight);
    (*g) = vel;
    return g;
}

auto LBM::getDensity()  -> Grid<float>* {
    if( getDensityKernel == NULL) return NULL;

    for( size_t i = 0; i < 9; i++) {
        getDensityKernel->input( src[i] );
    }

    getDensityKernel->output( dens.x() * dens.y(), reinterpret_cast<float*>(dens.data()));
    getDensityKernel->input( (int) gridWidth );
    getDensityKernel->input( (int) gridHeight );

    getDensityKernel->run(2, global_size, local_size );

    Grid<float>* g = new Grid<float>(gridWidth, gridHeight);
    (*g) = dens;
    return g;
}

void LBM::setTypes(const Grid<cell_t>&) {
}
}
