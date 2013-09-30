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
#include "EntityInstace"

namespace Rabenstein {
EntityInstace::EntityInstace(){}

EntityInstace::EntityInstace(const EntityInstace&)
    :scalarFactor(o.scalarFactor),
     size(o.x_size), y_size(o.y_size), width(o.width), height(o.height),
     pos(o.pos), rotation(o.rotation), mass(o.mass), moi(o.moi),
     cog(o.cog), speed(o.speed), sor(o.sor), type(o.type){}

EntityInstace::EntityInstace(std::shared_ptr<Entity> entityType,
                             float scalarFactor = 1)
    :type(entityType)
{
    resize(scalarFactor);
}

EntityInstace::resize(float factor){
    width  = factor * type->image.width();
    height = factor * type->image.height();
    //TODO fator zum umrechen pix in cell
    //x_size = foo * width;
    //y_size = foo * width;
    mass  = factor * type->mass;
    moi   = factor * type->moi;  // TODO ändert die sich wirklich
    cog.x = factor * type->cog.x;
    cog.y = factor * type->cog.y;

    scalarFactor = factor;
}
