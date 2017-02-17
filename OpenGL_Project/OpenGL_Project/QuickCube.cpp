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

//void drawTorus(double r = 0.07, double c = 0.15,
 //              int rSeg = 16, int cSeg = 8)
void QuickCube::drawTorus(double r, double c, int rSeg, int cSeg)
{
    glFrontFace(GL_CW);
    
    const double TAU = 2 * M_PI;
    
    for (int i = 0; i < rSeg; i++) {
        for (int j = 0; j <= cSeg; j++) {
            for (int k = 0; k <= 1; k++) {
                double s = (i + k) % rSeg + 0.5;
                double t = j % (cSeg + 1);
                
                double x = (c + r * cos(s * TAU / rSeg)) * cos(t * TAU / cSeg);
                double y = (c + r * cos(s * TAU / rSeg)) * sin(t * TAU / cSeg);
                double z = r * sin(s * TAU / rSeg);
                
                double u = (i + k) / (float) rSeg;
                double v = t / (float) cSeg;
                
                uv.push_back(glm::vec2(  u,  v));
                normal.push_back(glm::vec3(  2 * x,  2 * y, 2 * z ));
                vertss.push_back(glm::vec3(  2 * x,  2 * y, 2 * z ));
            }
        }
    }
   
    glFrontFace(GL_CCW);
    

}


void QuickCube::drawCube(){
    
    //drawTorus(0.4f, 0.2f, 100.0f, 100.0f);
    
    float size = 1.0;
    
    squarePyramideVerts = {
        //vertices positions                          //verts positions
        glm::vec3(  0,  size,  0 ),  // 0
        glm::vec3( 0,  size,  0 ),  // 1
        glm::vec3( -size, -size,  size ),  // 2
        glm::vec3(  size, -size,  size ),  // 3
        glm::vec3(  size, -size, -size ),  // 4
        glm::vec3( -size, -size, -size ),  // 5
        glm::vec3( 0,  size, 0 ),  // 6
        glm::vec3(  0,  size, 0 )  // 7
    };
    
    //glm::vec3 verts[8] = {
    verts = {
        //vertices positions                          //verts positions
        glm::vec3(  size,  size,  size ),  // 0 // front top right
        glm::vec3( -size,  size,  size ),  // 1 // front top left
        glm::vec3( -size, -size,  size ),  // 2 // front bottom left
        glm::vec3(  size, -size,  size ),  // 3 // front bottom right
        
//        glm::vec3(  size, 0.0f, -size ),  // 4 // mid back right
//        glm::vec3( -size, 0.0f, -size ),  // 5 // mid back left
//        glm::vec3( -size,  0.0f, -size ),  // 6 // mid front left 
//        glm::vec3(  size,  0.0f, -size ),   // 7 // mid front right
        
        glm::vec3(  size, -size, -size ),  // 4 // back bottom right
        glm::vec3( -size, -size, -size ),  // 5 // back bottom left
        glm::vec3( -size,  size, -size ),  // 6 // back top left 
        glm::vec3(  size,  size, -size )   // 7 // back top right
    };
    
    // Define the 8 vertices of a unit cube
    glm::vec3 colors[8] = {
        //vertices positions                          //colors
         glm::vec3( 1.0f, 1.0f, 1.0f ),  // 0
         glm::vec3( 0.0f, 1.0f, 1.0f ),  // 1
         glm::vec3( 0.0f, 0.0f, 1.0f ),  // 2
         glm::vec3( 1.0f, 0.0f, 1.0f ),  // 3
         glm::vec3( 1.0f, 0.0f, 0.0f ),  // 4
         glm::vec3( 0.0f, 0.0f, 0.0f ),  // 5
         glm::vec3( 0.0f, 1.0f, 0.0f ),  // 6
         glm::vec3( 1.0f, 1.0f, 0.0f ),  // 7
        
//         glm::vec3( 1.0f, 0.0f, 0.0f ),  // 8
//         glm::vec3( 0.0f, 0.0f, 0.0f ),  // 9
//         glm::vec3( 0.0f, 1.0f, 0.0f ),  // 10
//         glm::vec3( 1.0f, 1.0f, 0.0f )  // 11
    };
 
    
    
    // We have a color array and a vertex array 
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

//    glColorPointer( 3, GL_FLOAT, sizeof(VertexXYZColor), &(g_VertexBuffer[0].m_Color) );
//    glVertexPointer( 3, GL_FLOAT, sizeof(VertexXYZColor), &(g_VertexBuffer[0].m_Pos) );
//    
    //glVertexPointer(3, GL_FLOAT, 0, &vertss[0]);
    glVertexPointer(3, GL_FLOAT, 0, &verts[0]);
    
    glColorPointer(3, GL_FLOAT, 0, colors);
    
    glDrawElements( GL_QUADS, 24, GL_UNSIGNED_INT, g_IndexBuffer );
    
    //glDrawArrays(GL_QUAD_STRIP, 0, 102 * 2);
    
    // Cleanup states 
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    
    
    
    
}
