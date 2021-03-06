/* Copyright (C) 2013  Marco Heisig

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

#include "Player.hpp"
#include "Entity.hpp"
#include "EntityInstance.hpp"

namespace Rabenstein {

Player::Player(const QString& name, Entity* ship_type)
    : upPressed(false),
      downPressed(false),
      leftPressed(false),
      rightPressed(false),
      leftShootPressed(false),
      rightShootPressed(false),
      leftShootTimeout(0),
      leftShootCounter(0),
      rightShootTimeout(0),
      rightShootCounter(0),
      name(name),
      ship_type(ship_type),
      ship(nullptr),
      kills(0),
      deaths(0),
      handicap(1.0)
{
}

Player::~Player() {}

}
