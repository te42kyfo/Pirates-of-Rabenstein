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
            p->ship = new EntityInstance(p->ship_type, 10.0);
            p->ship->pos = Vec2D<float>(100.0, 100.0); // TODO
        }
        EntityInstance *ship = p->ship;
        float speed = 0.4;
        if(p->upPressed) speed += 0.4;
        if(p->downPressed) speed -= 0.4; // TODO
        if(p->leftPressed) ship->rotation -= 2.0;
        if(p->rightPressed) ship->rotation += 2.0;
        const float deg2rad = (2 * M_PI) / 360.0;
        Vec2D<float> n(cos(ship->rotation * deg2rad),
                sin(ship->rotation * deg2rad));
        ship->pos += n * speed;
        ship->pos += simulation->vel(ship->pos.x, ship->pos.y);
    }
}

void Game::gameLoop() {
    simulate();
    updatePositions();
    updateGL();
    updateTimer.start(10);
}

}
