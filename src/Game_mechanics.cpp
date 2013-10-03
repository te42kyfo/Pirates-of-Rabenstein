/* Copyright (C) 2013  Marco Heisig, Dominik Ernst, Frederic Simonis

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

const float cannon_offsets[] = {
    -5.0f, -2.0f, 1.0f, 7.0f
};

void Game::updatePositions() {
    for(auto p: players) {
        if(p->ship == nullptr) {
            p->ship = new EntityInstance(p->ship_type, 0.01);
            p->ship->pos = Vec2D<float>(simulation->gridWidth / 2.0f,
                                        simulation->gridHeight / 2.0f);
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
        Vec2D<float> new_pos = ship->pos + ship->speed;
        if(simulation->types(new_pos.x, new_pos.y) == LBM::cell_t::FLUID) {
            ship->pos = new_pos;
        } else {
            ship->speed = {0.0f, 0.0f};
        }

        // Shoot left
        if(p->leftShootPressed) {
            if( p->leftShootTimeout <= 0) {
                bullets.push_back( {bullet, 0.18} );
                bullets.back().pos = ship->pos + heading*cannon_offsets[p->leftShootCounter % 4];
                bullets.back().speed = { -heading.y*7, heading.x*7 };
                p->leftShootTimeout = 30;
                bullets.back().lifeTime = 16;
                p->leftShootCounter++;
            }
            p->leftShootPressed = false;
         }
        p->leftShootTimeout--;

        // Shoot right
        if(p->rightShootPressed) {
            if( p->rightShootTimeout <= 0) {
                bullets.push_back( {bullet, 0.18} );
                bullets.back().pos = ship->pos;
                bullets.back().pos = ship->pos + heading*cannon_offsets[p->rightShootCounter % 4];
                bullets.back().speed = { heading.y*7, -heading.x*7 };
                p->rightShootTimeout = 30;
                bullets.back().lifeTime = 16;
                p->rightShootCounter++;
            }
            p->rightShootPressed = false;
        }
        p->rightShootTimeout--;

    }
    for( vector<EntityInstance>::iterator b = bullets.begin();
         b != bullets.end(); b++) {
        b->speed *=  0.97;
        b->pos += b->speed;
       
        bool player_proximity = false;
        for( auto p : players) {
            if( (b->pos - p->ship->pos).abs() < 16 && b->lifeTime < 14) {
                player_proximity = true;
                explosions.push_back( {explosion, 0.0} );
                explosions.back().pos = p->ship->pos;
                explosions.back().lifeTime = 10;
                p->deaths++;
                p->ship->pos = Vec2D<float>(simulation->gridWidth / 2.0f,
                                            simulation->gridHeight / 2.0f);
                for( size_t i = 0; i < 30; i++) {
                    debriss.push_back( {debris, 0.1} );
                    debriss.back().pos = p->ship->pos;
                    float angle = (rand()%3600)/10.0;
                    debriss.back().speed = { sin(angle)*(rand()%3),
                                             cos(angle)*(rand()%3) };
                    debriss.back().lifeTime = 100+rand()%50;
                }             
            }
        } 
        (b->lifeTime)--;
        if( b->lifeTime  <= 0 || player_proximity ) {

            explosions.push_back( {explosion, 0.0} );
            explosions.back().pos = b->pos;
            explosions.back().lifeTime = 6;

            for( size_t i = 0; i < 30; i++) {
                debriss.push_back( {debris, 0.1} );
                debriss.back().pos = b->pos;
                float angle = (rand()%3600)/10.0;
                debriss.back().speed = { sin(angle)*(rand()%3),
                                         cos(angle)*(rand()%3) };
                debriss.back().lifeTime = 100+rand()%50;
            }             
            
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
