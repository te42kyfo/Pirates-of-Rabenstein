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
#include <cmath>
#include "Game.hpp"
#include "LBM.hpp"
#include "Player.hpp"
using namespace std;

namespace Rabenstein {

void Game::simulate() {
    simulation->one_iteration();
    simulation->one_iteration();
    simulation->one_iteration();
    simulation->downloadVelocity();
}

void Game::updatePositions() {
    for(auto p: players) {
        if(p->ship == nullptr) {
            p->ship = new EntityInstance(p->ship_type, 0.01);
            p->ship->pos = Vec2D<float>(50.0, 50.0); // TODO
            p->ship->width = 1.0f; // TODO
            p->ship->height = 1.0f; // TODO
            p->ship->scalarFactor = 0.03f; // TODO
            p->ship->moi = 1.0f; // TODO
        }

        const float deg2rad = (2 * M_PI) / 360.0;

        EntityInstance *ship = p->ship;
        Vec2D<float> acc(0,0);
        Vec2D<float> heading( cos(ship->rotation * deg2rad),
                              sin(ship->rotation * deg2rad) );

        if(p->upPressed) acc += heading*0.05;
        if(p->leftPressed) ship->rotation += 2.0;
        if(p->rightPressed) ship->rotation -= 2.0;
        
        acc += (simulation->vel(ship->pos.x, ship->pos.y)*6 - ship->speed)/15.0;
        
        ship->speed += acc;
        ship->pos += ship->speed;

    }
}

void Game::gameLoop() {
    simulate();
    updatePositions();
    updateGL();
    updateTimer.start(10);
}

}
