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

#ifndef RABENSTEIN__NEW_GAME_WINDOW_HPP
#define RABENSTEIN__NEW_GAME_WINDOW_HPP
#include <QtGui/QDialog>

namespace Rabenstein {
class NewGameWindow : public QDialog
{
      Q_OBJECT

public:
    NewGameWindow(QWidget* parent);
    virtual ~NewGameWindow();
};
}
#endif // RABENSTEIN__NEW_GAME_WINDOW_HPP