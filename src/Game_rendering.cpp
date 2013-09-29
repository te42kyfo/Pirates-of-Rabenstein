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

using namespace std;

namespace Rabenstein {

void Game::initializeGL() {
    glewInit();
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_NORMALIZE);

}

void Game::resizeGL(int width, int height) {
    // TODO
}

void drawIndexedVertices( vector<GLfloat>& vertices,
                          vector<GLfloat>& normals,
                          vector<GLfloat>& colors,
                          vector<GLuint>& indices ) {
    GLuint vboVerticesId;                              // ID of VBO
    GLuint vboIndicesId;                              // ID of VBO
    GLuint vboNormalsId;                              // ID of VBO
    GLuint vboColorsId;                              // ID of VBO

    glEnableClientState(GL_VERTEX_ARRAY);             // activate vertex coords array
    glEnableClientState(GL_NORMAL_ARRAY);             // activate vertex coords array
    glEnableClientState(GL_COLOR_ARRAY);             // activate vertex coords array
        
    glGenBuffers(1, &vboIndicesId);    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB, vboIndicesId); // for indices   
    glBufferData( GL_ELEMENT_ARRAY_BUFFER_ARB,
                  indices.size()*sizeof(GLuint) , indices.data(),
                  GL_STATIC_DRAW_ARB);

    glGenBuffers(1, &vboVerticesId);
    glBindBuffer( GL_ARRAY_BUFFER_ARB, vboVerticesId);         // for vertex coordinates    
    glBufferData( GL_ARRAY_BUFFER_ARB,
                  vertices.size()*sizeof(GLfloat) , vertices.data(),
                  GL_STATIC_DRAW_ARB);
    glVertexPointer(3, GL_FLOAT, 0, 0);               // last param is offset, not ptr
    
    glGenBuffers(1, &vboNormalsId);    
    glBindBuffer(GL_ARRAY_BUFFER_ARB, vboNormalsId); // for indices   
    glBufferData( GL_ARRAY_BUFFER_ARB,
                  normals.size()*sizeof(GLfloat) , normals.data(),
                  GL_STATIC_DRAW_ARB);
    glNormalPointer( GL_FLOAT, 0, 0);               // last param is offset, not ptr
 
    glGenBuffers(1, &vboColorsId);    
    glBindBuffer(GL_ARRAY_BUFFER_ARB, vboColorsId); // for indices   
    glBufferData( GL_ARRAY_BUFFER_ARB,
                  colors.size()*sizeof(GLfloat) , colors.data(),
                  GL_STATIC_DRAW_ARB);
    glColorPointer(3, GL_FLOAT, 0, 0);               // last param is offset, not ptr
    
    
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    
    glDisableClientState(GL_VERTEX_ARRAY);            // deactivate vertex array
    glDisableClientState(GL_NORMAL_ARRAY);            // deactivate vertex array

    glBindBuffer(GL_ARRAY_BUFFER_ARB, 0);
    glBindBuffer(GL_NORMAL_ARRAY, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);

    glDeleteBuffers(1, &vboNormalsId);
    glDeleteBuffers(1, &vboVerticesId);
    glDeleteBuffers(1, &vboIndicesId);
}

void Game::paintGL() {

    glColor3f(1.0, 1.0, 1.0);
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION) ;
    glLoadIdentity();
    glOrtho(0.0, 1.0, 1.0, 0.0, -5, 5);

    GLfloat light_position[] = { 0.5, 0.5, -30.0, 0.0 };
    glShadeModel (GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHT0);
  
    glLightfv(GL_LIGHT0, GL_POSITION, light_position); 
      



    shared_ptr< Grid<Vec2D<float>>> velocity ( simulation->get_velocity_grid());
    shared_ptr< Grid< float >> pressure ( simulation->get_density_grid());
 


    vector<GLfloat> vertices;
    vector<GLuint> indices;
    vector<GLfloat> normals;
    vector<GLfloat> colors;

    for(size_t iy = 0; iy < velocity->y(); iy ++) {
        for(size_t ix = 0; ix < velocity->x(); ix ++) {
            vertices.push_back( ix );
            vertices.push_back( iy );
            vertices.push_back( ((*pressure)(ix, iy)-1.0)*3.0 );

            float dx = (*pressure)(ix-1, iy) - (*pressure)(ix+1, iy);
            float dy = (*pressure)(ix, iy-1) - (*pressure)(ix, iy+1);

            normals.push_back( dx*100.0 );
            normals.push_back( dy*100.0 );
            normals.push_back( -0.1 );

            colors.push_back( 1.0);//((*pressure)(ix, iy)-0.85)*4.0 );
            colors.push_back( 1.0);//-((*pressure)(ix, iy)-0.85)*4.0 );
            colors.push_back( 1.0 );
            
            if( iy > 0 && ix > 0) {
                indices.push_back( iy*velocity->x()+ix-1 );
                indices.push_back( iy*velocity->x()+ix );
                indices.push_back( (iy-1)*velocity->x()+ix );

                indices.push_back( iy*velocity->x()+ix-1 );                
                indices.push_back( (iy-1)*velocity->x()+ix );
                indices.push_back( (iy-1)*velocity->x()+ix-1 );
           }
        }
    }


    /*
    glTranslatef( 0.0f, -0.5, 0.0f);
    glRotatef( 10.0, 1.0, 0.0, 0.0);
    glTranslatef( 0.0f, 0.5, 0.0f);
    glTranslatef( 0.5f, 0.5, 0.0f);
    glRotatef( frame_counter/5.0, 0.0, 0.0, 1.0);
    glTranslatef( -0.5f, -0.5, 0.0f);
    */
    glScalef( 1.0f/(velocity->x()-1), 1.0f/(velocity->y()-1), 1.0);
    drawIndexedVertices(vertices, normals, colors, indices);

  

}
}


