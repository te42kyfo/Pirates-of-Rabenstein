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

#include <QtGui/QAction>
#include <QtGui/QMenuBar>
#include <QKeySequence>
#include <QFileDialog>
#include <QTabWidget>
#include <QGLWidget>
#include <CL/cl.hpp>
#include <vector>
#include <sstream>
#include <iostream>
#include "GameWindow.hpp"

using namespace std;

namespace Rabenstein {

GameWindow::GameWindow(size_t x, size_t y, size_t refinement) {
    setWindowTitle(tr("Rabenstein"));
    setMinimumSize(320, 240);

    glWidget = new QGLWidget(QGLFormat(QGL::Rgba | QGL::DoubleBuffer), this);
    setCentralWidget(glWidget);

    /* creating file menu*/
    newA = new QAction(tr("&New Game"), this);
    newA->setShortcuts(QKeySequence::New);
    newA->setStatusTip(tr("Start a new game"));
    connect(newA, SIGNAL(triggered()), this, SLOT(newGame()));

    exitA = new QAction(tr("E&xit"), this);
    exitA->setShortcuts(QKeySequence::Quit);
    exitA->setStatusTip(tr("Exit the game"));
    connect(exitA, SIGNAL(triggered()), this, SLOT(close()));

    fullscreenA = new QAction(tr("Fullscreen"), this);
    fullscreenA->setShortcut(QKeySequence(Qt::Key_F11));
    fullscreenA->setStatusTip(tr("Toggle fullscreen mode"));
    connect(fullscreenA, SIGNAL(triggered()), this, SLOT(fullscreen()));

    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newA);
    fileMenu->addSeparator();
    fileMenu->addAction(fullscreenA);
    fileMenu->addAction(exitA);
    /* opencl menu */
    openclMenu = menuBar()->addMenu(tr("OpenCL"));

    std::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);
    for(cl::Platform& p : platforms) {
        stringstream pstr;
        pstr << p.getInfo<CL_PLATFORM_NAME>();
        QMenu *menu = new QMenu(tr(pstr.str().c_str()), this);
        cl_context_properties cps[3] = {
            CL_CONTEXT_PLATFORM,
            (cl_context_properties)p(),
            0};
        cl::Context all(CL_DEVICE_TYPE_ALL, cps);
        std::vector<cl::Device> devices =
            all.getInfo<CL_CONTEXT_DEVICES>();
        for(cl::Device d : devices) {
            stringstream dstr;
            dstr << d.getInfo<CL_DEVICE_NAME>();
            /* TODO */
        }
        openclMenu->addMenu(menu);
    }

    /* help menu */
    aboutA = new QAction(tr("&About"), this);
    aboutA->setStatusTip(tr("Information about the Rabenstein software"));
    connect(aboutA, SIGNAL(triggered()), this, SLOT(about()));

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutA);
}

void GameWindow::newGame(){
    // TODO initialize new Game
    // should prompt for Level file, OpenCL device
}

void GameWindow::fullscreen(){
    this->setWindowState(this->windowState() ^ Qt::WindowFullScreen);
}

void GameWindow::about(){
    // TODO spawn about window
}

void GameWindow::contextSelected(cl::Context* context){
    
}
}

