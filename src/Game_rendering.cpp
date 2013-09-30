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
#include <memory>

using namespace std;

namespace Rabenstein {

    string readShaderFile(const string fileName) {
        try {
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
        } catch( std::exception& e) {
            std::cout << "readShaderFile: " << e.what() << "\n";
            return string();
        }
        
    }

    void Game::loadShader(string vshader, string fshader) {
        GLcharARB log[5000];
        GLsizei length;        

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
        
        glGetInfoLogARB( lic_vertex, 5000, &length, log);
        if( length > 0) std::cout << "Vertex Shader: "  << log << "\n";
        glGetInfoLogARB( lic_fragment, 5000, &length, log);
        if( length > 0) std::cout << "Fragment Shader: " << log << "\n";
        glGetInfoLogARB( lic_program, 5000, &length, log);
        if( length > 0) std::cout << "Progrgam: " << log << "\n";
               
    }


    GLuint Game::loadTexture(QString path) {
        // Load Level texture
        GLuint texture_handle;

        QImage input_image( path );
        input_image = convertToGLFormat( input_image);

        glGenTextures(1, &texture_handle);
        glBindTexture( GL_TEXTURE_2D, texture_handle);

        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA,
                      input_image.width(), input_image.height(),
                      0, GL_RGBA, GL_UNSIGNED_BYTE, input_image.bits());
        return texture_handle;
    }

void Game::initializeGL() {
    glewInit();
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_NORMALIZE);

    
    level_texture = loadTexture(level_texture_path);
    bg_texture = loadTexture("../data/noise.png");

    
    loadShader("../src/lic.vert", "../src/lic.frag");

    
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
        
    GLuint velocity_texture_handle;
    glGenTextures(1, &velocity_texture_handle);
    glBindTexture( GL_TEXTURE_2D, velocity_texture_handle);
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RG32F,
                  velocity->x(), velocity->y(),
                  0, GL_RG, GL_FLOAT, velocity->data() );
 

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glOrtho(0, 1, 1, 0, -5, 5);

    glColor3f(1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT |
			GL_DEPTH_BUFFER_BIT);


    glMatrixMode(GL_PROJECTION) ;
    glLoadIdentity();

    glUseProgram(lic_program);

    //texture1
    GLint bg_tex_uloc = glGetUniformLocation(lic_program, "bg_tex");
    glActiveTexture(GL_TEXTURE0);
    glBindTexture( GL_TEXTURE_2D, bg_texture);
    glUniform1i( bg_tex_uloc, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //texture2
    GLint level_tex_uloc = glGetUniformLocation(lic_program, "level_tex");
    glActiveTexture(GL_TEXTURE1);
    glBindTexture( GL_TEXTURE_2D, level_texture);
    glUniform1i( level_tex_uloc, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    
    //texture3
    GLint velocity_uloc = glGetUniformLocation(lic_program, "velocity");
    glActiveTexture(GL_TEXTURE2);
    glBindTexture( GL_TEXTURE_2D, velocity_texture_handle);
    glUniform1i( velocity_uloc, 2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    //Frame counter    
    static int frame_counter = 0;
    GLint frame_counter_uloc = glGetUniformLocation(lic_program, "frame_counter");
    glUniform1i( frame_counter_uloc, frame_counter++);


    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0, 0.0, 0.0);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0, 0.0, 0.0);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0, 1.0, 0.0);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0, 1.0, 0.0);
    glEnd();

    glDeleteTextures(1, &velocity_texture_handle);

}
}


