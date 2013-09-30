/* Copyright (C) 2013  Marco Heisig, Christopher Bross

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

#ifndef RABENSTEIN__ENTITY_HPP
#define RABENSTEIN__ENTITY_HPP
#include <cstddef>
#include <QImage>
#include "Vec2D.hpp"

namespace Rabenstein {

enum Entity_Type{
    FOO,
    BAA
};

struct Entity {

    Entity();
    Entity(const Entity&);
    Entity(Entity_Type, QString image_path);
    ~Entity();
    //void updatePos(float x, float y);
    //void updateRot(float rot);

    Vec2D<float> pos;       // Uper Left Corner
    float x_size, y_size;   // Cells //TODO do we need it hear?
    int width, height;      // Pixels
    float rotation;
    float mass;
    float moi;              // Moment of inertia //TODO berechnung
    Vec2D<float> cog;       // Relative To pos
    Vec2D<float> speed;
    Entity_Type type;
    QImage image;
};
}
#endif // RABENSTEIN__ENTITY_HPP
