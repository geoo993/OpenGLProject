//
//  QuickCube.hpp
//  OpenGL_Project
//
//  Created by GEORGE QUENTIN on 11/02/2017.
//  Copyright © 2017 LEXI LABS. All rights reserved.
//

#ifndef QuickCube_hpp
#define QuickCube_hpp

#include "Common.h"

// Vertex definition
struct VertexXYZColor
{
    glm::vec3 m_Pos;
    glm::vec3 m_Color;
};


class QuickCube : public VertexXYZColor{
    
public :
    QuickCube();
    QuickCube(int size);
    ~QuickCube();
    
    void drawVertices(float size);
    void drawCube();
    
    
private:
    // Define the 8 vertices of a unit cube
    //VertexXYZColor g_VertexBuffer[8];
    std::vector <VertexXYZColor>  g_VertexBuffer;
    
    // Define the vertex indices for the cube.
    GLuint g_IndexBuffer[24] = {
        0, 1, 2, 3,                 // Front face
        7, 4, 5, 6,                 // Back face
        6, 5, 2, 1,                 // Left face
        7, 0, 3, 4,                 // Right face
        7, 6, 1, 0,                 // Top face
        3, 2, 5, 4,                 // Bottom face
    };
    
};

#endif /* QuickCube_hpp */
