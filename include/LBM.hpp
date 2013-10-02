/* Copyright (C) 2013  Dominik Ernst

   This file is part of Feldrand.

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

#ifndef RABENSTEIN__LBM_HPP
#define RABENSTEIN__LBM_HPP

#include "OpenClHelper/OpenCLHelper.h"
#include "OpenClHelper/CLKernel.h"
#include "Grid.hpp"
#include "Vec2D.hpp"
#include <QImage>

namespace Rabenstein {

class LBM  {

    enum class cell_t : int {
        FLUID = 0,
        NO_SLIP = 1,
        SOURCE = 2,
        COPY = 3
    };

public:
    LBM(size_t grid_width, size_t grid_height);
    LBM(QString path, size_t ratio);
    virtual ~LBM();
    void init();
    void one_iteration();
    void do_clear();
    void downloadVelocity();
    void downloadDensity();
    void setTypes(const Grid<cell_t>& types);

    size_t gridWidth;
    size_t gridHeight;
    Grid<Vec2D<float>> vel;
    Grid<float> dens;

private:
    void setFields(const size_t ix, const size_t iy,
                   const float* val, const int type);

    CLKernel* getVelocityKernel;
    CLKernel* getDensityKernel;
    CLKernel* simulationStepKernel;
    OpenCLHelper* cl;
    CLArrayFloat* dst[9];
    CLArrayFloat* src[9];
    CLArrayInt* flag_field;

    size_t global_size[2];
    size_t local_size[2];

};
}
#endif // RABENSTEIN__LBM_HPP
