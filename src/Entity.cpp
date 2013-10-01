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

#include "Entity.hpp"
#include <QImage>
#include "Vec2D.hpp"
#include <cassert>
#include "Game.hpp"

using namespace std;
namespace Rabenstein {

Entity::Entity(){}

Entity::Entity(Entity_Type type, QString image_path)
    :type(type), image(image_path)
{
    width  = image.width();
    height = image.height();
    //x_size = foo * width; TODO foo festlegen
    //y_size = foo * height;
    //moi = boo TODO was machen wir damit
    assert(image.hasAlphaChannel());

    cog.x = 0;
    cog.y = 0;
    mass  = 0;
    for (int i = 0; i < image.height(); ++i) {
        for (int j = 0; j < image.width(); ++j){
            cog.x += j * qAlpha(image.pixel(j, i));
            cog.y += i * qAlpha(image.pixel(j, i));
            mass  +=     qAlpha(image.pixel(j, i));
        }
    }

    cog.x /= 255;
cog.y /= 255;
mass  /= 255;
texture_loaded = false;
path = image_path;
}

Entity::~Entity(){}
} // end namespace
