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

#include <iostream>
#include "Game.hpp"
#include "LBM.hpp"
using namespace std;

namespace Rabenstein {

void Game::simulate() {
    simulation->one_iteration();
    simulation->one_iteration();
    simulation->one_iteration();
}

void Game::updatePositions() {
    // TODO
}

void Game::gameLoop() {
    simulate();
    // download velocity data
    updatePositions();
    // collision detection
    // destruction, damage & respawn
    // send back type field
    updateGL();
    updateTimer.start(10);
}

}
