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
#include "Vec2D.hpp"
#include <QImage>
#include <cassert>
#include "GL/glew.h"
#include "GL/glu.h"

using namespace std;
namespace Rabenstein {

Entity::Entity(){}


Entity::Entity(const Entity& o)
	: pos(o.pos), x_size(o.x_size), y_size(o.y_size),
	  width(o.width), height(o.height), rotation(o.rotation), mass(o.mass),
	  cog(o.cog), speed(o.speed), type(o.type), image(o.image){}

Entity::Entity(Entity_Type type, QString image_path):
	mass(0), cog(0,0), type(type), image(image_path)
{
	width  = image.width();
	height = image.height();

	assert(image.hasAlphaChannel());

	for (int i = 0; i < image.height(); ++i) {
		for (int j = 0; j < image.width(); ++j){
			cog.x += j * qAlpha(image.pixel(j, i));
			cog.y += i * qAlpha(image.pixel(j, i));
			mass += qAlpha(image.pixel(j, i));
		}
	}
	cog.x /= 255;
	cog.y /= 255;
	mass  /= 255;
}

Entity::~Entity(){}
} // end namespace
