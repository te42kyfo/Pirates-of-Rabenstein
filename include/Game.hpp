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
#include <QTimer>
#include <QString>
#include <vector>
#include "Entity.hpp"
#include "EntityInstance.hpp"
#include <QGLShader>
#include "Grid.hpp"

namespace Rabenstein {
class LBM;
class Player;

class Game : public QGLWidget {
    Q_OBJECT

public: // constructors
    Game(const QString& path, QWidget *parent = 0);

protected: // events, see /usr/include/qt4/QtGui/{qevent.h, qwidget.h}
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseDoubleClickEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void wheelEvent(QWheelEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void keyReleaseEvent(QKeyEvent *event);
private slots: // game mechanics
    void gameLoop();
private:
    void simulate();
    void updatePositions();
    LBM* simulation;
    std::vector<Player*> players;
    Entity* bullet;
    Entity* explosion;
    Entity* debris;

    std::vector<EntityInstance> bullets;
    std::vector<EntityInstance> explosions;
    std::vector<EntityInstance> debriss;

private:
    QTimer updateTimer;
protected: // rendering, see /usr/include/qt4/QtOpenGL/qgl.h
    virtual void initializeGL();
    virtual void resizeGL(int width, int height);
    virtual void paintGL();
    GLuint level_texture;
    GLuint bg_texture;
    QString level_texture_path;

    void loadShader(std::string vshader, std::string fshader);
    GLenum lic_program, lic_vertex, lic_fragment;
public:
    GLuint loadTexture(QString path);
};
}
#endif // RABENSTEIN__GAME_HPP

