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

#include "Game.hpp"
using namespace std;

namespace Rabenstein {

void Game::initializeGL() {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void Game::resizeGL(int width, int height) {
    // TODO
}

void drawArrow() {
    glBegin( GL_TRIANGLES );
    glVertex3f( 0.5,  0.0, 0.0f);
    glVertex3f( 0.0,  0.2, 0.0f);
    glVertex3f( 0.0, -0.2, 0.0f);

    glVertex3f(-0.5, -0.05, 0.0f);
    glVertex3f( 0.0, -0.05, 0.0f);
    glVertex3f( 0.0,  0.05, 0.0f);

    glVertex3f(-0.5, -0.05, 0.0f);
    glVertex3f(-0.5,  0.05, 0.0f);
    glVertex3f( 0.0,  0.05, 0.0f);
    glEnd();
}

void Game::paintGL() {

    glColor3f(1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION) ;
    glLoadIdentity();

    // the drawing routines work on a unity square
    // with (0.0, 0.0) being the upper left corner
    glOrtho(0.0, 1.0, 1.0, 0.0, -5, 5);


    shared_ptr< Grid<Vec2D<float>>> velocity ( simulation->get_velocity_grid());


    const size_t cells_per_dim = 12;
    const size_t cells_per_arrow = cells_per_dim * cells_per_dim;
    for(size_t iy = 0; iy < velocity->y() - cells_per_dim; iy += cells_per_dim) {
        for(size_t ix = 0; ix < velocity->x() - cells_per_dim; ix += cells_per_dim) {
            Vec2D<float> mean = Vec2D<float>();
            // gather mean direction over all relevant cells
            for(size_t dy = 0; dy < cells_per_dim; dy += 1) {
                for(size_t dx = 0; dx < cells_per_dim; dx += 1) {
                    mean += (*velocity)(ix + dx, iy + dy);
                }
            }
            mean /= (float) cells_per_arrow;
            if(mean.abs() < 0.0001) continue;
            glPushMatrix();
            Vec2D<float> point = {(float)ix / (float)velocity->x(),
                                  (float)iy / (float)velocity->y()};
            glTranslatef(point.x, point.y, 0.0f);
            glScalef(1.0 / (float) velocity->x() * cells_per_dim,
                     1.0 / (float) velocity->y() * cells_per_dim, 1.0f);
            glTranslatef(0.5f, 0.5f, 0.0f);
            glRotatef(atan2(mean.y,mean.x) / (2.0f * M_PI) * 360.0f,
                      0.0f, 0.0f, 1.0f);
            

            drawArrow();
            glPopMatrix();
        }
    }

}
}


