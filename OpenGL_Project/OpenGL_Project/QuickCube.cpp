//
//  QuickCube.cpp
//  OpenGL_Project
//
//  Created by GEORGE QUENTIN on 11/02/2017.
//  Copyright © 2017 LEXI LABS. All rights reserved.
//

#include "QuickCube.hpp"


QuickCube::QuickCube(){
   
}
QuickCube::QuickCube(int size){
    drawVertices(size);
}
QuickCube::~QuickCube(){
    
}

void QuickCube::drawVertices(float size){
    
    // Define the 8 vertices of a unit cube
    g_VertexBuffer = {
        //vertices positions                          //colors
        { glm::vec3(  size,  size,  size ), glm::vec3( 1.0f, 1.0f, 1.0f ) }, // 0
        { glm::vec3( -size,  size,  size ), glm::vec3( 0.0f, 1.0f, 1.0f ) }, // 1
        { glm::vec3( -size, -size,  size ), glm::vec3( 0.0f, 0.0f, 1.0f ) }, // 2
        { glm::vec3(  size, -size,  size ), glm::vec3( 1.0f, 0.0f, 1.0f ) }, // 3
        { glm::vec3(  size, -size, -size ), glm::vec3( 1.0f, 0.0f, 0.0f ) }, // 4
        { glm::vec3( -size, -size, -size ), glm::vec3( 0.0f, 0.0f, 0.0f ) }, // 5
        { glm::vec3( -size,  size, -size ), glm::vec3( 0.0f, 1.0f, 0.0f ) }, // 6
        { glm::vec3(  size,  size, -size ), glm::vec3( 1.0f, 1.0f, 0.0f ) }, // 7
    };
 
}

void QuickCube::drawCube(){
    

    // We have a color array and a vertex array 
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glColorPointer( 3, GL_FLOAT, sizeof(VertexXYZColor), &(g_VertexBuffer[0].m_Color) );
    glVertexPointer( 3, GL_FLOAT, sizeof(VertexXYZColor), &(g_VertexBuffer[0].m_Pos) );
    
    //std::cout << "posss" << sizeof(VertexXYZColor) << " " << sizeof(g_VertexBuffer) << std::endl;
    // Send data : 24 vertices 
    glDrawElements( GL_QUADS, 24, GL_UNSIGNED_INT, g_IndexBuffer );
    
    // Cleanup states 
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    
    
}
