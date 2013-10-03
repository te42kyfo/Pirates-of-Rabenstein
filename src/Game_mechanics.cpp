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
            p->ship->scalarFactor = 0.2f; // TODO
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

        if(p->shootPressed) {
            if( p->shootTimeout <= 0) {
                bullets.push_back( {bullet, 0.18} );
                bullets.back().pos = ship->pos;
                bullets.back().speed = { heading.y*7, -heading.x*7 };
                p->shootTimeout = 20;
                bullets.back().lifeTime = 16;
            }
            p->shootPressed = false;
         }
        p->shootTimeout--;

    }
    for( vector<EntityInstance>::iterator b = bullets.begin();
         b != bullets.end(); b++) {
        b->speed *=  0.97;
        b->pos += b->speed;
        if( --(b->lifeTime) <= 0) {

            size_t debris_count = 8;
            for( auto p : players) {
                if( (b->pos - p->ship->pos).abs() < 20) {
                    explosions.push_back( {explosion, 0.0} );
                    explosions.back().pos = p->ship->pos;
                    explosions.back().lifeTime = 10;
                    p->deaths++;
                    p->ship->pos = {40, 40};
                    debris_count += 20;
                }
            }
            for( size_t i = 0; i < debris_count; i++) {
                debriss.push_back( {debris, 0.08} );
                debriss.back().pos = b->pos;
                float angle = (rand()%3600)/10.0;

                debriss.back().speed = { sin(angle)*(rand()%3),
                                         cos(angle)*(rand()%3) };
                debriss.back().lifeTime = 100+rand()%50;
            }

            explosions.push_back( {explosion, 0.0} );
            explosions.back().pos = b->pos;
            explosions.back().lifeTime = 6;
            
            b = bullets.erase(b);
            b--;

        }
    }
    for( vector<EntityInstance>::iterator e = explosions.begin();
         e != explosions.end(); e++) {

        e->scalarFactor = sin( 6-e->lifeTime)*0.1;

        if( --(e->lifeTime) <= 0) {
            
            e = explosions.erase(e);
            e--;
        }
    }
    for( vector<EntityInstance>::iterator e = debriss.begin();
         e != debriss.end(); e++) {
        if( --(e->lifeTime) <= 0 ||
            e->pos.x > simulation->vel.x()-1 ||
            e->pos.y > simulation->vel.y()-1 ||
            e->pos.x < 0 ||
            e->pos.y < 0) {
            e = debriss.erase(e);
            e--;
            continue;
        }

        Vec2D<float> acc
            = (simulation->vel(e->pos.x, e->pos.y)*6 - e->speed)/15.0;

        e->speed += acc;
        e->pos += e->speed;



    }

}

void Game::gameLoop() {
    simulate();
    updatePositions();
    updateGL();
    updateTimer.start(10);
}

}
