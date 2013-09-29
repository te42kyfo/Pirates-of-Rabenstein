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
#include "BGK_OCL.hpp"
using namespace std;

namespace Rabenstein {

Game::Game(std::string path, QWidget *parent)
    : QGLWidget(QGLFormat(QGL::Rgba | QGL::DoubleBuffer), parent),
      frame_counter(0)
{
    simulation = new BGK_OCL( 10.0f, 1.0f, 300, 300);

    QObject::connect(&updateTimer, SIGNAL(timeout()), this, SLOT(gameLoop()));
    updateTimer.start( 0 );
}

Game::Game(Game& other, QWidget *parent)
    : QGLWidget(QGLFormat(QGL::Rgba | QGL::DoubleBuffer), parent),
      frame_counter(0)
{
    // TODO clone existing game with different OpenCL context
}
}
