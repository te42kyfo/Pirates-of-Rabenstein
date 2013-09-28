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

void Game::paintGL() {

    glColor3f(1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION) ;
    glLoadIdentity();

    // the drawing routines work on a unity square
    // with (0.0, 0.0) being the upper left corner
    glOrtho(0.0, 1.0, 1.0, 0.0, -5, 5);

    glTranslatef(0.5f, 0.5f, 0.0f);
    glRotatef( 360.0/1000.0*frame_counter, 0.0, 0.0f, 1.0f);
    glTranslatef(-0.5f, -0.5f, 0.0f);

    glBegin(GL_TRIANGLES);
    glVertex3f( 0.1f, 0.1f, 0.0f);
    glVertex3f( 0.1f, 0.9f, 0.0f);
    glVertex3f( 0.9f, 0.0f, 0.0f);
    glEnd();
}
}


