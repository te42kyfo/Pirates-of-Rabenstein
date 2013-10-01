/* Copyright (C) 2013  Christopher Bross

This file is part of Rabenstein.

Rabenstein is free software: you can redistribute it and/or modify it under the
terms of the GNU Affero General Public License as published by the Free
Software Foundation, either version 3 of the License, or (at your option) any
later version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more
details.

You should have received a copy of the GNU Affero General Public License along
with this program.  If not, see <http://www.gnu.org/licenses/>. */

#ifndef RABENSTEIN__ENTITY_INSTANCE_HPP
#define RABENSTEIN__ENTITY_INSTANCE_HPP
#include <cstddef>
#include <memory>
#include "Vec2D.hpp"
#include "Entity.hpp"

namespace Rabenstein {

struct EntityInstance {
    EntityInstance();
    EntityInstance(const EntityInstance&);
    EntityInstance( Entity*, float scalarFactor = 1);

    void resize(float);
    float scalarFactor;
    float x_size, y_size;   // Cells //TODO factot
    int width, height;      // Pixels
    Vec2D<float> pos;       // Uper Left Corner
    float rotation;         // Clockwise
    float mass;
    float moi;              // Moment of inertia //TODO berechnung
    Vec2D<float> cog;       // Center of grafity relative to pos
    Vec2D<float> speed;     // in x,y direction
    float sor;              // speed of rotation (Clockwise)
    Entity* type;
};
}
#endif // RABENSTEIN__ENTITY_INSTANCE_HPP
