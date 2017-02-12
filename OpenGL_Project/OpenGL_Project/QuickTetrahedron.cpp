//
//  QuickTetrahedron.cpp
//  OpenGL_Project
//
//  Created by GEORGE QUENTIN on 12/02/2017.
//  Copyright © 2017 LEXI LABS. All rights reserved.
//

#include "QuickTetrahedron.hpp"

QuickTetrahedron::QuickTetrahedron(){
    
}
QuickTetrahedron::QuickTetrahedron(float size){
    drawVertices(size);
}
QuickTetrahedron::~QuickTetrahedron(){
    
}

void QuickTetrahedron::drawVertices(float size){
    
    // Draw the tetrahedron.  It is a four sided figure, so when defining it
    // with a triangle strip we have to repeat the last two vertices.
    vertices =
    {
        0.0f, size, 0.0f,
        -size, -(size/2.0f), size,
        1.0f, -(size/2.0f), size,
        0.0f, -(size/2.0f), -(size ),
        0.0f, size , 0.0f,
        -size, -(size/2.0f), size
    };
    
    colors =
    {
        1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 0.0f,
    };
    
}



void QuickTetrahedron::drawTetrahedron(){
    
    // We have a color array and a vertex array 
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    
    glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);
    glColorPointer(3, GL_FLOAT, 0, &colors[0]);
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);
    
    // Cleanup states 
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
}
