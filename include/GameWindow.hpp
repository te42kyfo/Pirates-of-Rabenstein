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

#ifndef RABENSTEIN__GAME_WINDOW_HPP
#define RABENSTEIN__GAME_WINDOW_HPP
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
class Game;

class GameWindow : public QMainWindow {
    Q_OBJECT
public:
    GameWindow();
private slots:
    void newGame();
    void fullscreen();
    void about();
private:
    size_t x;
    size_t y;
    QMenu *fileMenu, *openclMenu, *helpMenu;
    QAction *newA, *exitA, *fullscreenA, *aboutA;
    Game *game;
};
}
#endif // RABENSTEIN__GAME_WINDOW_HPP
