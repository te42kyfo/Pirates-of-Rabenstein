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

QT_BEGIN_NAMESPACE
class QLabel;
class QFileDialog;
class QGridLayout;
QT_END_NAMESPACE


namespace Rabenstein {
class Game;

class LobbyWidget : public QWidget
{
      Q_OBJECT

public:
    LobbyWidget(QWidget* parent);
    virtual ~LobbyWidget();
public slots:
    void mapSelected(const QString& path);
    void done();
signals:
    void setupFinished(Game *game);
private:
    QLabel *level;
    QFileDialog *files;
    QGridLayout *grid;
};
}
#endif // RABENSTEIN__LOBBY_WIDGET_HPP
