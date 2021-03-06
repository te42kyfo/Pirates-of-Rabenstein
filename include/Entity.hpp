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
#include "GL/gl.h"

namespace Rabenstein {

enum class Entity_Type{
    FOAM,
    SHIP
};

struct Entity {
    Entity();
    Entity(Entity_Type, QString image_path);
    ~Entity();
    //void updatePos(float x, float y);
    //void updateRot(float rot);

    float x_size, y_size;   // Cells //TODO do we need it here?
    int width, height;      // Pixels
    float mass;
    float moi;              // Moment of inertia //TODO calculation
    Vec2D<float> cog;       // Relative To pos
    Entity_Type type;
    QImage image;
    GLuint texture_handle;
    bool texture_loaded;
    QString path;
};
}
#endif // RABENSTEIN__ENTITY_HPP
