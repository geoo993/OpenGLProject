//
//  QuickTrianglesGube.hpp
//  OpenGL_Project
//
//  Created by GEORGE QUENTIN on 12/02/2017.
//  Copyright © 2017 LEXI LABS. All rights reserved.
//

#ifndef QuickTrianglesGube_hpp
#define QuickTrianglesGube_hpp

#include "Common.h"

class QuickTrianglesGube {
    
public :
    QuickTrianglesGube();
    QuickTrianglesGube(float size);
    ~QuickTrianglesGube();
    
    void drawVertices(float size);
    void drawCube(const GLuint &shaderProgram);
    
private:
    
    GLuint VertexArrayID;
    GLuint vertexbuffer;
    GLuint colorbuffer;
    /*
    // Define the 8 vertices of a unit cube
    const float  vertexBufferData[] = {
        // Top face
        1.0f, 1.0f, -1.0f,  // Top-right of top face
        -1.0f, 1.0f, -1.0f,  // Top-left of top face
        -1.0f, 1.0f,  1.0f,  // Bottom-left of top face
        1.0f, 1.0f,  1.0f,  // Bottom-right of top face
        
        // Bottom face
        1.0f, -1.0f, -1.0f, // Top-right of bottom face
        1.0f, -1.0f,  1.0f, // Bottom-right of bottom face
        -1.0f, -1.0f,  1.0f, // Bottom-left of bottom face
        -1.0f, -1.0f, -1.0f, // Top-left of bottom face
        
        // Front face
        1.0f,  1.0f, 1.0f,  // Top-Right of front face
        -1.0f,  1.0f, 1.0f,  // Top-left of front face
        -1.0f, -1.0f, 1.0f,  // Bottom-left of front face
        1.0f, -1.0f, 1.0f,  // Bottom-right of front face
        
        // Back face
        1.0f, -1.0f, -1.0f, // Bottom-Left of back face
        -1.0f, -1.0f, -1.0f, // Bottom-Right of back face
        -1.0f,  1.0f, -1.0f, // Top-Right of back face
        1.0f,  1.0f, -1.0f, // Top-Left of back face
        
        // Left face
        -1.0f,  1.0f,  1.0f,  // Top-Right of left face
        -1.0f,  1.0f, -1.0f,  // Top-Left of left face
        -1.0f, -1.0f, -1.0f,  // Bottom-Left of left face
        -1.0f, -1.0f,  1.0f,  // Bottom-Right of left face
        
        // Right face
        1.0f,  1.0f, -1.0f,  // Top-Right of left face
        1.0f,  1.0f, 1.0f,  // Top-Left of left face
        1.0f, -1.0f, 1.0f,  // Bottom-Left of left face
        1.0f, -1.0f, -1.0f  // Bottom-Right of left face
        
    };
    
    const float colorBufferData[] = { 
        
        1.0f,  0.0f,  0.0f,   // Red
        0.0f,  1.0f,  0.0f,   // Green
        0.0f,  0.0f,  1.0f,  // Blue
        1.0f,  1.0f,  0.0f, // Yellow
        0.0f,  1.0f,  1.0f,  // Cyan
        1.0f,  0.0f,  1.0f,  // Violet
        1.0f,  0.5f,  0.0f, // Orange
        0.5f,  1.0f,  0.5f,  // 
        0.0f,  0.5f,  1.0f  // Violet
        
    };
    */
};


#endif /* QuickTrianglesGube_hpp */
