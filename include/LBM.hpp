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

namespace Rabenstein {

    class LBM  {
    public:
        LBM();
        LBM(size_t grid_width, size_t grid_height);
        virtual ~LBM();
        void init();
        void one_iteration();
        void do_clear();
        auto getVelocity() -> Grid<Vec2D<float>>*;
        auto getDensity()  -> Grid<float>*;
        size_t const gridWidth;
        size_t const gridHeight;
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

        Grid<Vec2D<float>> vel;
        Grid<float> dens;
    };
}
#endif // RABENSTEIN__LBM_HPP
