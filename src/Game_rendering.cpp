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

#include <memory>
#include "GL/glew.h"
#include "GL/glu.h"
#include "Game.hpp"
#include "LBM.hpp"
#include <QFileInfo>

using namespace std;

namespace Rabenstein {

    string readShaderFile(const string fileName) {
        std::ifstream shaderFile( fileName.c_str() );
        
        // find the file size
        shaderFile.seekg(0,std::ios::end);
        std::streampos          length = shaderFile.tellg();
        shaderFile.seekg(0,std::ios::beg);

        // read whole file into a vector:
        std::vector<char>       buffer(length);
        shaderFile.read(&buffer[0],length);
        
        // return the shader string
        return std::string( buffer.begin(), buffer.end() );
    }

    void Game::LoadShader(string vshader, string fshader) {

        
        string vertex_shader = readShaderFile( vshader );
        string fragment_shader = readShaderFile( fshader );
        char const * my_fragment_shader_source = fragment_shader.c_str();
        char const * my_vertex_shader_source = vertex_shader.c_str();

        // Create Shader And Program Objects
        lic_program = glCreateProgramObjectARB();
        lic_vertex = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
        lic_fragment = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
        
        // Load Shader Sources
        glShaderSourceARB(lic_vertex, 1, &my_vertex_shader_source, NULL);
        glShaderSourceARB(lic_fragment, 1, &my_fragment_shader_source, NULL);
 
        // Compile The Shaders
        glCompileShaderARB(lic_vertex);
        glCompileShaderARB(lic_fragment);
        
        // Attach The Shader Objects To The Program Object
        glAttachObjectARB(lic_program, lic_vertex);
        glAttachObjectARB(lic_program, lic_fragment);
        
        // Link The Program Object
        glLinkProgramARB(lic_program);   
        
     
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

    
    LoadShader("../src/lic.vert", "../src/lic.frag");

    
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
    glDisable(GL_DEPTH_TEST);

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

    


    glUseProgramObjectARB(lic_program);



    glBegin(GL_QUADS);
    glTexCoord3f(0.0f, 1.0f, 0.5f); glVertex3f(0.0, 0.0, 0.0);
    glTexCoord3f(1.0f, 1.0f, 0.5f); glVertex3f(1.0, 0.0, 0.0);
    glTexCoord3f(1.0f, 0.0f, 0.5f); glVertex3f(1.0, 1.0, 0.0);
    glTexCoord3f(0.0f, 0.0f, 0.5f); glVertex3f(0.0, 1.0, 0.0);
    glEnd();

}
}


