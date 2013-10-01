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
    velocity = simulation->getVelocity();

}

void Game::updatePositions() {
    for(auto& e: entities) {
        e.pos.x += (*velocity)( e.pos.x*velocity->x(),
                                e.pos.y*velocity->y() ).x*0.008;
        e.pos.y += (*velocity)( e.pos.x*velocity->x(),
                                e.pos.y*velocity->y() ).y*0.008;

    }
}

void Game::spawn() {
    for( size_t i = 0; i < 1; i++) {
        entities.push_back( EntityInstance( &(entityClasses.back()), 10.0) );
        entities.back().pos.x = (rand()%1000)/1000.0;
        entities.back().pos.y = (rand()%1000)/1000.0;
    }
}


void Game::gameLoop() {
    simulate();
    spawn();
    updatePositions();
    // collision detection
    // destruction, damage & respawn
    // send back type field
    

    updateGL();
    updateTimer.start(10);
}

}
