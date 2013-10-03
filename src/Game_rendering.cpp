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
#include <QFileInfo>
#include <QColor>
#include <GL/glew.h>
#include <GL/glu.h>
#include "Game.hpp"
#include "LBM.hpp"
#include "Player.hpp"

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

GLuint Game::loadLevelTexture(QString path) {
    // Load Level texture
    GLuint texture_handle;
    QImage input_image(path);
    input_image = convertToGLFormat(input_image);

    // hide color values of 255
    for(int iy = 0; iy < input_image.width(); ++iy) {
        for(int ix = 0; ix < input_image.height(); ++ix) {
            QRgb c = input_image.pixel(iy, ix);
            if((qRed(c) == 255 && qBlue(c) ==   0 && qGreen(c) ==   0) ||
               (qRed(c) ==   0 && qBlue(c) == 255 && qGreen(c) ==   0) ||
               (qRed(c) ==   0 && qBlue(c) ==   0 && qGreen(c) == 255))
                input_image.setPixel(iy, ix, qRgba(0, 0, 0, 0));
        }
    }

    if( input_image.width() == 0)
        std::cout << "loadLevelTexture: error\n";

    glGenTextures(1, &texture_handle);
    glBindTexture( GL_TEXTURE_2D, texture_handle);

    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA,
                  input_image.width(), input_image.height(),
                  0, GL_RGBA, GL_UNSIGNED_BYTE, input_image.bits());
    std::cout << "loaded " << texture_handle << "\n";
    return texture_handle;
}

GLuint Game::loadTexture(QString path) {
    // Load Level texture
    GLuint texture_handle;
    QImage input_image( path );
    input_image = convertToGLFormat(input_image);

    if( input_image.width() == 0)
        std::cout << "loadTexture: error\n";

    glGenTextures(1, &texture_handle);
    glBindTexture( GL_TEXTURE_2D, texture_handle);

    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA,
                  input_image.width(), input_image.height(),
                  0, GL_RGBA, GL_UNSIGNED_BYTE, input_image.bits());
    std::cout << "loaded " << texture_handle << "\n";
    return texture_handle;
}

void Game::initializeGL() {
    glewInit();
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_NORMALIZE);

    level_texture = loadLevelTexture(level_texture_path);
    bg_texture = loadTexture("../data/noise.png");

    loadShader("../src/lic.vert", "../src/lic.frag");
    this->setFocus();
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
                  simulation->gridWidth, simulation->gridHeight,
                  0, GL_RG, GL_FLOAT, simulation->vel.data());

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    glOrtho(0, 1, 1, 0, -5, 5);

    glColor3f(1.0, 1.0, 1.0);

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
    static int frame_counter = 79;

    frame_counter++;
    // the integration kernel walks from -1.0 to 1.0
    const int period = 500;
    const float phalfinv = 1.0f / float(period / 2);
    float t1 = float((frame_counter +                0) % period) * phalfinv - 1.0f;
    float t2 = float((frame_counter + (1 * period) / 4) % period) * phalfinv - 1.0f;
    float t3 = float((frame_counter + (2 * period) / 4) % period) * phalfinv - 1.0f;
    float t4 = float((frame_counter + (3 * period) / 4) % period) * phalfinv - 1.0f;

    glUniform1f( glGetUniformLocation(lic_program, "t1"), t1);
    glUniform1f( glGetUniformLocation(lic_program, "t2"), t2);
    glUniform1f( glGetUniformLocation(lic_program, "t3"), t3);
    glUniform1f( glGetUniformLocation(lic_program, "t4"), t4);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0, 0.0, 0.0);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0, 0.0, 0.0);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0, 1.0, 0.0);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0, 1.0, 0.0);
    glEnd();

    glDeleteTextures(1, &velocity_texture_handle);

    glUseProgram(0);

    float maxw = simulation->gridWidth;
    float maxh = simulation->gridHeight;
    for(auto& p : players) {
        EntityInstance *e = p->ship;
        glEnable(GL_TEXTURE_2D);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture( GL_TEXTURE_2D, e->type->texture_handle);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glLoadIdentity();
        glOrtho(0, maxw, maxh, 0, -5, 5);

        if( ! e->type->texture_loaded ) {
            e->type->texture_handle = loadTexture(e->type->path);
            e->type->texture_loaded = true;
        }
        
        glTranslatef(e->pos.x, maxh - e->pos.y, 0.0);
        
        glRotatef(e->rotation+90, 0.0f, 0.0f, -1.0f);
        glTranslatef( e->type->cog.x*e->scalarFactor*-2.0,
                      e->type->cog.y*e->scalarFactor*2.0, 0);


        float sx = e->type->width * e->scalarFactor;
        float sy = e->type->height * e->scalarFactor;

        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(  0,   0, 0.4);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( sx,   0, 0.4);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( sx,  sy, 0.4);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(  0,  sy, 0.4);
        glEnd();
    }
    for( auto& b : bullets) {
        glEnable(GL_TEXTURE_2D);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture( GL_TEXTURE_2D, b.type->texture_handle);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glLoadIdentity();
        glOrtho(0, maxw, maxh, 0, -5, 5);

        if( ! b.type->texture_loaded ) {
            b.type->texture_handle = loadTexture(b.type->path);
            b.type->texture_loaded = true;
        }
        
        glTranslatef(b.pos.x, maxh - b.pos.y, 0.0);

        glTranslatef( b.type->cog.x*b.scalarFactor*-2.0,
                      b.type->cog.y*b.scalarFactor*2.0, 0);

        float sx = b.type->width * b.scalarFactor;
        float sy = b.type->height * b.scalarFactor;

        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(  0,   0, 0.4);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( sx,   0, 0.4);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( sx,  sy, 0.4);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(  0,  sy, 0.4);
        glEnd();
    }
    for( auto& b : explosions) {

        glDisable(GL_TEXTURE_2D);
        glLoadIdentity();
        glOrtho(0, 1, 1, 0, -5, 5);
        glColor4f(1.0, 1.0, 1.0, b.scalarFactor);
        glBegin(GL_QUADS);
        glVertex3f(  0,       0, 0);
        glVertex3f( maxw,     0, 0);
        glVertex3f( maxw,  maxh, 0);
        glVertex3f(  0,    maxh, 0);
        glEnd();
        
            
        glEnable(GL_TEXTURE_2D);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture( GL_TEXTURE_2D, b.type->texture_handle);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glLoadIdentity();
        glOrtho(0, maxw, maxh, 0, -5, 5);

        glColor4f(1.0, 1.0, 1.0, 1.0);

        if( ! b.type->texture_loaded ) {
            b.type->texture_handle = loadTexture(b.type->path);
            b.type->texture_loaded = true;
        }
        
        glTranslatef(b.pos.x, maxh - b.pos.y, 0.0);

        glTranslatef( b.type->cog.x*b.scalarFactor*-2.0,
                      b.type->cog.y*b.scalarFactor*2.0, 0);

        float sx = b.type->width * b.scalarFactor;
        float sy = b.type->height * b.scalarFactor;

        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(  0,   0, 0.4);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( sx,   0, 0.4);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( sx,  sy, 0.4);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(  0,  sy, 0.4);
        glEnd();
    }
    for( auto& b : debriss) {
        glEnable(GL_TEXTURE_2D);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture( GL_TEXTURE_2D, b.type->texture_handle);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glLoadIdentity();
        glOrtho(0, maxw, maxh, 0, -5, 5);

        if( ! b.type->texture_loaded ) {
            b.type->texture_handle = loadTexture(b.type->path);
            b.type->texture_loaded = true;
        }
        
        glTranslatef(b.pos.x, maxh - b.pos.y, 0.0);

        glTranslatef( b.type->cog.x*b.scalarFactor*-2.0,
                      b.type->cog.y*b.scalarFactor*2.0, 0);

        float sx = b.type->width * b.scalarFactor;
        float sy = b.type->height * b.scalarFactor;

        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(  0,   0, 0.4);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( sx,   0, 0.4);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( sx,  sy, 0.4);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(  0,  sy, 0.4);
        glEnd();
    }

}
}


