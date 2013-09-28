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
#include <QMainWindow>
#include "config.hpp"

QT_BEGIN_NAMESPACE
class QMenu;
class QAction;
class QGLWidget;
QT_END_NAMESPACE

namespace cl {
class Context;
}

namespace Rabenstein {
class GameWindow : public QMainWindow {
    Q_OBJECT
public:
    GameWindow(size_t x, size_t y, size_t refinement);

private slots:
    void newGame();
    void fullscreen();
    void about();
    void contextSelected(cl::Context* context);
private:
    size_t x;
    size_t y;
    QMenu *fileMenu, *openclMenu, *helpMenu;
    QAction *newA, *exitA, *fullscreenA, *aboutA;
    QGLWidget *glWidget;
};
}
#endif // RABENSTEIN__GAME_HPP
