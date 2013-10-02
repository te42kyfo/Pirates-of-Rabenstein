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

#include <QKeyEvent>
#include <iostream>
#include "Game.hpp"
#include "Player.hpp"
using namespace std;

namespace Rabenstein {

void Game::mousePressEvent(QMouseEvent *event) {
    // TODO
}

void Game::mouseReleaseEvent(QMouseEvent *event) {
    // TODO
}

void Game::mouseDoubleClickEvent(QMouseEvent *event) {
    // TODO
}

void Game::mouseMoveEvent(QMouseEvent *event) {
    // TODO
}

void Game::wheelEvent(QWheelEvent *event) {
    // TODO
}

void Game::keyPressEvent(QKeyEvent *event) {
    if(event->key() == Qt::Key_W) players[0]->upPressed = true;
    if(event->key() == Qt::Key_A) players[0]->leftPressed = true;
    if(event->key() == Qt::Key_S) players[0]->downPressed = true;
    if(event->key() == Qt::Key_D) players[0]->rightPressed = true;
    if(event->key() == Qt::Key_Up) players[1]->upPressed = true;
    if(event->key() == Qt::Key_Left) players[1]->leftPressed = true;
    if(event->key() == Qt::Key_Down) players[1]->downPressed = true;
    if(event->key() == Qt::Key_Right) players[1]->rightPressed = true;
}

void Game::keyReleaseEvent(QKeyEvent *event) {
    if(event->key() == Qt::Key_W) players[0]->upPressed = false;
    if(event->key() == Qt::Key_A) players[0]->leftPressed = false;
    if(event->key() == Qt::Key_S) players[0]->downPressed = false;
    if(event->key() == Qt::Key_D) players[0]->rightPressed = false;
    if(event->key() == Qt::Key_Up) players[1]->upPressed = false;
    if(event->key() == Qt::Key_Left) players[1]->leftPressed = false;
    if(event->key() == Qt::Key_Down) players[1]->downPressed = false;
    if(event->key() == Qt::Key_Right) players[1]->rightPressed = false;
}
}
