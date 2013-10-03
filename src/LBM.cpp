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
    1.2f/36.0f, 1.2f/9.0f, 1.2f/36.0f,
    1.2f/9.0f,  3.0f/9.0f, 1.2f/9.0f,
    1.2f/36.0f, 1.2f/9.0f, 1.2f/36.0f
};

const float drain[] = {
    0.7f/36.0f, 0.7f/9.0f, 0.7f/36.0f,
    0.7f/9.0f,   5.0f/9.0f, 0.7f/9.0f,
    0.7f/36.0f, 0.7f/9.0f, 0.7f/36.0f
};
}

LBM::LBM(QString path, size_t ratio)
    : getVelocityKernel(NULL),
      getDensityKernel(NULL),
      simulationStepKernel(NULL)
{
    QImage image(path);

    gridWidth = image.width() / ratio;
    gridHeight = image.height() / ratio;
    vel = Grid<Vec2D<float>> (gridWidth, gridHeight);
    dens = Grid<float> (gridWidth, gridHeight);
    types = Grid<cell_t> (gridWidth, gridHeight);

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


    for (size_t y = 0; y < gridHeight; ++y) {
        for (size_t x = 0; x < gridWidth; ++x) {
            size_t ix = x *ratio;
            size_t iy = (gridHeight-y-1) * ratio;

            if( qAlpha( image.pixel(ix, iy)) == 0) {
                setFields(x, y, fluid,  (int) cell_t::FLUID);
            } else {
                setFields(x, y, fluid,  (int) cell_t::NO_SLIP);
                if( qRed( image.pixel(ix, iy)) == 255) {
                    setFields(x, y, source,  (int) cell_t::SOURCE);
                }
                if( qGreen( image.pixel(ix, iy)) == 255) {
                    setFields(x, y, drain,  (int) cell_t::SOURCE);
                }
                if( qBlue( image.pixel(ix, iy)) == 255) {
                    setFields(x, y, fluid,  (int) cell_t::COPY);
                }
            }
        }
    }

    for(size_t i = 0; i < 9;i++) {
        dst[i]->copyToDevice();
        src[i]->copyToDevice();
    }
    for(size_t iy = 0; iy < gridHeight; ++iy) {
        for(size_t ix = 0; ix < gridWidth; ++ix) {
            types(ix, iy) = (cell_t)(*flag_field)[iy*gridWidth + ix];
        }
    }
    flag_field->copyToDevice();
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

void LBM::setType(const size_t ix, const size_t iy, const int type) {
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


    for(size_t i = 0; i < 9;i++) {
        dst[i]->copyToDevice();
        src[i]->copyToDevice();
    }

    flag_field->copyToDevice();
}

void LBM::downloadVelocity() {
    if( getVelocityKernel == NULL) return;

    for( size_t i = 0; i < 9; i++) {
        getVelocityKernel->input( src[i] );
    }

    getVelocityKernel->output( vel.x() * vel.y() * 2, reinterpret_cast<float*>(vel.data()));
    getVelocityKernel->input((int)gridWidth);
    getVelocityKernel->input((int)gridHeight);

    getVelocityKernel->run(2, global_size, local_size );
}

void LBM::copyUp() {
   flag_field->copyToDevice();
}

void LBM::downloadDensity() {
    if( getDensityKernel == NULL) return;

    for( size_t i = 0; i < 9; i++) {
        getDensityKernel->input( src[i] );
    }

    getDensityKernel->output( dens.x() * dens.y(), reinterpret_cast<float*>(dens.data()));
    getDensityKernel->input((int) gridWidth);
    getDensityKernel->input((int) gridHeight);

    getDensityKernel->run(2, global_size, local_size );
}

void LBM::setTypes(const Grid<cell_t>&) {
    // TODO
}


}
