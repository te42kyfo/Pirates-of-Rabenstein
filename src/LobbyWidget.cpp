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

#include <CL/cl.hpp>
#include <QtGui/QGridLayout>
#include <QtGui/QLabel>
#include <QtGui/QFileDialog>
#include <QtGui/QPixmap>
#include <QtGui/QPushButton>
#include "LobbyWidget.hpp"
#include "Game.hpp"
#include "Entity.hpp"
#include "Player.hpp"

namespace Rabenstein {
LobbyWidget::LobbyWidget(QWidget* parent) : QWidget(parent),
                                            context(nullptr),
                                            device(nullptr)
{
    grid = new QGridLayout(this);

    files  = new QFileDialog();
    files->setNameFilter(tr("Images (*.png *.jpg)"));

    level = new QLabel();
    connect(files, SIGNAL(fileSelected(const QString&)),
            this, SLOT(mapSelected(const QString&)));

    QPushButton *done = new QPushButton(tr("Arr!"));
    connect(done, SIGNAL(clicked()),
            this, SLOT(done()));

    grid->addWidget(files, 0, 0, 3, 1);
    grid->setColumnStretch(0, 2);
    grid->setColumnMinimumWidth(0, 100);
    grid->setColumnMinimumWidth(1, 20);
    grid->addWidget(level, 0, 2, 2, 1);
    grid->setColumnMinimumWidth(2, 100);
    grid->setColumnStretch(2, 2);

    grid->setRowMinimumHeight(1, 100);
    grid->addWidget(done, 2, 2);
}

LobbyWidget::~LobbyWidget() {
}

void LobbyWidget::mapSelected(const QString& path) {
    QPixmap *pixmap = new QPixmap(path);
    level->setPixmap(pixmap->scaledToWidth(level->x(),
                                           Qt::SmoothTransformation));
    /* restarting the files dialog */
    files  = new QFileDialog();
    grid->addWidget(files, 0, 0, 3, 1);
    files->setNameFilter(tr("Images (*.png *.jpg)"));
    connect(files, SIGNAL(fileSelected(const QString&)),
            this, SLOT(mapSelected(const QString&)));
    /* storing the path */
    this->path = path;
}

void LobbyWidget::openCLSelected() {
}

void LobbyWidget::widthSelected() {
}

void LobbyWidget::heightSelected() {
}

void LobbyWidget::done() {
    if(path.length() > 0) { // TODO catch more
        emit setupFinished(new Game(path));
    }
}

}

