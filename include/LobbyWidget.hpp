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

#ifndef RABENSTEIN__LOBBY_WIDGET_HPP
#define RABENSTEIN__LOBBY_WIDGET_HPP
#include <QtGui/QDialog>
#include <QString>
#include <GL/gl.h>
#include <vector>

QT_BEGIN_NAMESPACE
class QLabel;
class QFileDialog;
class QGridLayout;
QT_END_NAMESPACE

namespace cl {
class Context;
class Device;
}

namespace Rabenstein {
class Game;
class Player;
class Entity;


/* The layout of the Lobby Widget:

  | (0, 0) file chooser dialog  | (0, 1) Space | (0, 2) currently selected level |
  | (1, 0) file chooser dialog  | (1, 1) Space | (1, 2) currently selected level |
  | (2, 0) file chooser dialog  | (2, 1) Space | (2, 2) done button              |
  |----------------------+--------------+-----------------------------+----------|
  | Name1                | Color        | Ship                        | Handicap |
  | Name2                | Color        | Ship                        | Handicap |
  | Name3                | Color        | Ship                        | Handicap |
  | Name4                | Color        | Ship                        | Handicap |*/
class LobbyWidget : public QWidget
{
      Q_OBJECT

public:
    LobbyWidget(QWidget* parent);
    virtual ~LobbyWidget();
public slots:
    void mapSelected(const QString& path);
    void openCLSelected();
    void widthSelected();
    void heightSelected();
    void done();
signals:
    void setupFinished(Game *game);
private:
    QLabel *level;
    QFileDialog *files;
    QGridLayout *grid;
private: // data required for game
    QString path;
    cl::Context* context;
    cl::Device* device;
    std::vector<Player*> player;
};
}
#endif // RABENSTEIN__LOBBY_WIDGET_HPP
