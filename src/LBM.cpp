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
enum class cell_type : int {
    FLUID = 0,
        NO_SLIP = 1,
        SOURCE = 2
        };

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
      vel( grid_width*grid_height*2 ),
      density( grid_width*grid_height)
{
    init();
}

LBM::~LBM() {
    delete getVelocityKernel;
    delete getDensityKernel;
    delete simulationStepKernel;
    delete cl;
}

// OpenCL initialization needs to be doen from the same thread that calls
// the kernels later
void LBM::init() {

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
    simulationStepKernel->input( flag_field );

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
            setFields(ix, iy, fluid, (int) cell_type::FLUID);
        }
    }

    for(size_t iy = 0; iy < gridHeight; ++iy) {
        setFields(0,           iy, source,  (int) cell_type::SOURCE);
        setFields(gridWidth-1, iy, drain, (int) cell_type::SOURCE);
    }

    for(size_t ix = 0; ix < gridWidth; ++ix) {
        setFields(ix, 0,            fluid, (int) cell_type::NO_SLIP);
        setFields(ix, gridHeight-1, fluid, (int) cell_type::NO_SLIP);
    }
    for(size_t i = 0; i < 9;i++) {
        dst[i]->copyToDevice();
        src[i]->copyToDevice();
    }

    flag_field->copyToDevice();
}
/*
  void LBM::do_draw(int x, int y,
  shared_ptr<const Grid<mask_t>> mask_ptr,
  cell_t type) {
  int cx = x;
  int cy = y;


  for(size_t i = 0; i < 9;i++) {
  if( !dst[i]->isOnHost())
  dst[i]->copyToHost();
  if( !src[i]->isOnHost())
  src[i]->copyToHost();
  }
  if( !flag_field->isOnHost())
  flag_field->copyToHost();

  const Grid<mask_t>& mask = *(mask_ptr);

  int upper_left_x = cx - (mask.x() / 2);
  int upper_left_y = cy - (mask.y() / 2);
  for(size_t iy = 0; iy < mask.y(); ++iy) {
  for(size_t ix = 0; ix < mask.x(); ++ix) {
  int sx = upper_left_x + ix;
  int sy = upper_left_y + iy;
  if(sx < 0 || sx >= (int)gridWidth ||
  sy < 0 || sy >= (int)gridHeight) continue;

  if(mask_t::IGNORE == mask(ix, iy)) continue;
  if( type == cell_t::OBSTACLE &&
  (*flag_field)[sy*gridWidth+sx] == (int) cell_type::FLUID) {
  setFields(sx, sy, fluid, (int) cell_type::NO_SLIP);
  }
  if( type == cell_t::FLUID &&
  (*flag_field)[sy*gridWidth+sx] == (int) cell_type::NO_SLIP) {
  setFields(sx, sy, fluid, (int) cell_type::FLUID);
  }
  }
  }
  for(size_t i = 0; i < 9;i++) {
  dst[i]->copyToDevice();
  src[i]->copyToDevice();
  }
  flag_field->copyToDevice();
  }
*/
auto LBM::get_velocity_grid() -> Grid<Vec2D<float>>* {

    if( getVelocityKernel == NULL) return NULL;

    for( size_t i = 0; i < 9; i++) {
        getVelocityKernel->input( src[i] );
    }

    getVelocityKernel->output( vel.size(), vel.data() );
    getVelocityKernel->input( (int) gridWidth );
    getVelocityKernel->input( (int) gridHeight );

    getVelocityKernel->run(2, global_size, local_size );

    Grid<Vec2D<float>>* g(new Grid<Vec2D<float>>(gridWidth, gridHeight));
    for(size_t iy = 0; iy < gridHeight; ++iy) {
        for(size_t ix = 0; ix < gridWidth; ++ix) {
            (*g)(ix, iy) = Vec2D<float> { vel[iy * gridWidth*2 +ix*2],
                                          vel[iy * gridWidth*2 +ix*2 +1] };
        }
    }
    return g;
}

    auto LBM::get_density_grid()  -> Grid<float>* {
    if( getDensityKernel == NULL) return NULL;


    for( size_t i = 0; i < 9; i++) {
        getDensityKernel->input( src[i] );
    }

    getDensityKernel->output( density.size(), density.data() );
    getDensityKernel->input( (int) gridWidth );
    getDensityKernel->input( (int) gridHeight );

    getDensityKernel->run(2, global_size, local_size );

    Grid<float>* g(new Grid<float>(gridWidth, gridHeight));
    for(size_t iy = 0; iy < gridHeight; ++iy) {
        for(size_t ix = 0; ix < gridWidth; ++ix) {
            (*g)(ix, iy) = density[iy * gridWidth +ix];
        }
    }
    return g;
}

/*auto LBM::get_type_grid()     -> Grid<cell_t>* {
  Grid<cell_t>* g(new Grid<cell_t>(gridWidth, gridHeight));
  for(size_t iy = 0; iy < gridHeight; ++iy) {
  for(size_t ix = 0; ix < gridWidth; ++ix) {
  (*g)(ix, iy) = cell_t::FLUID;
  }
  }
  return g;
  }*/
}
