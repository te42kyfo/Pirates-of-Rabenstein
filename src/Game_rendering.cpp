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


#include "GL/glew.h"
#include "GL/glu.h"
#include "Game.hpp"
#include "LBM.hpp"
#include <QFileInfo>

using namespace std;

namespace Rabenstein {


    void Game::LoadShader(QString vshader, QString fshader) {
        if(lic_frag) {
            lic_frag->release();
            lic_frag->removeAllShaders();
        } else {
            lic_frag = new QGLShaderProgram;        
        }
// load and compile vertex shader
        QFileInfo vsh(vshader);
        if( !vsh.exists() ) {
            cout << "Vertex Shader source file not found.\n";
            return;
        }

        VertexShader = new QGLShader(QGLShader::Vertex);
        if( !VertexShader->compileSourceFile(vshader)) {
            cout << "Vertex Shader Error\n";// << VertexShader->log();
            return;
        }
        lic_frag->addShader(VertexShader);
    


        // load and compile fragment shader
        QFileInfo fsh(fshader);
        if(!fsh.exists()) {
            cout << "Fragment Shader source file not found.";
            return;
        }    

        FragmentShader = new QGLShader(QGLShader::Fragment);
        if( !FragmentShader->compileSourceFile(fshader)) {
            cout << "Fragment Shader Error\n";// << FragmentShader->log();
            return;
        }
        lic_frag->addShader(FragmentShader);
    
        if(!lic_frag->link())  {
            cout << "Shader Program Linker Error\n";// << lic_frag->log();
        }
        else lic_frag->bind();
    }

void Game::initializeGL() {
    glewInit();
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_NORMALIZE);

    glEnable(GL_TEXTURE_2D);


    // Load Level texture
    QImage level_image( level_texture_path );
    level_image = convertToGLFormat(level_image);

    glGenTextures(1, &level_texture);
    glBindTexture( GL_TEXTURE_2D, level_texture);

    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA,
                  level_image.width(), level_image.height(),
                  0, GL_RGBA, GL_UNSIGNED_BYTE, level_image.bits());


   
}

void Game::resizeGL(int width, int height) {
    float swidth  = (float)simulation->gridWidth;
    float sheight = (float)simulation->gridHeight;
    float wscale  = (float)width / swidth;
    float hscale  = (float)height / sheight;
    int w, h;
    int w_offset, h_offset;
    if(wscale < hscale) {
        w = width; h = sheight * wscale;
        h_offset = (height - h) / 2;
        w_offset = 0;
    } else {
        h = height; w = swidth * hscale;
        w_offset = (width - w) / 2;
        h_offset = 0;
    }
    glViewport((GLsizei)w_offset, (GLsizei)h_offset,
               (GLsizei)w, (GLsizei)h);
}





void Game::paintGL() {

    glColor3f(1.0, 1.0, 1.0);
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 1.0, 1.0, 0.0, -5, 5);


    glEnable(GL_TEXTURE_2D);
    glBindTexture( GL_TEXTURE_2D, level_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBegin(GL_QUADS);
    glTexCoord3f(0.0f, 1.0f, 0.5f); glVertex3f(0.0, 0.0, 0.0);
    glTexCoord3f(1.0f, 1.0f, 0.5f); glVertex3f(1.0, 0.0, 0.0);
    glTexCoord3f(1.0f, 0.0f, 0.5f); glVertex3f(1.0, 1.0, 0.0);
    glTexCoord3f(0.0f, 0.0f, 0.5f); glVertex3f(0.0, 1.0, 0.0);
    glEnd();

    
    glDisable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
    glTexCoord3f(0.0f, 1.0f, 0.5f); glVertex3f(0.0, 0.0, 0.0);
    glTexCoord3f(1.0f, 1.0f, 0.5f); glVertex3f(1.0, 0.0, 0.0);
    glTexCoord3f(1.0f, 0.0f, 0.5f); glVertex3f(1.0, 1.0, 0.0);
    glTexCoord3f(0.0f, 0.0f, 0.5f); glVertex3f(0.0, 1.0, 0.0);
    glEnd();



}
}


