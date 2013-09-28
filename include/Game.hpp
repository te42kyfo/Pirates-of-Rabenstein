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

#ifndef RABENSTEIN__GAME_HPP
#define RABENSTEIN__GAME_HPP

#include <cstddef>
#include <string>
#include <QGLWidget>
#include <CL/cl.hpp>
#include <vector>
#include "Entity.hpp"

namespace Rabenstein {
class Game : public QGLWidget {
    Q_OBJECT

public: // constructors
    Game(cl::Context context, std::string path, QWidget *parent = 0);
    Game(cl::Context context, Game &other, QWidget *parent = 0);

protected: // events, see /usr/include/qt4/QtGui/{qevent.h, qwidget.h}
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *);
    virtual void mouseDoubleClickEvent(QMouseEvent *);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void wheelEvent(QWheelEvent *);
    virtual void keyPressEvent(QKeyEvent *);
    virtual void keyReleaseEvent(QKeyEvent *);
private: // game mechanics
    void gameLoop();
    std::vector<Entity> entities;
protected: // rendering, see /usr/include/qt4/QtOpenGL/qgl.h
    virtual void initializeGL();
    virtual void resizeGL(int width, int height);
    void render();
private: // simulation
    void simulate();
    void updatePositions();
};
}
#endif // RABENSTEIN__GAME_HPP

