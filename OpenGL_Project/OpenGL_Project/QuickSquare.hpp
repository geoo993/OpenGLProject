//
//  QuickSquare.hpp
//  OpenGL_Project
//
//  Created by GEORGE QUENTIN on 11/02/2017.
//  Copyright © 2017 LEXI LABS. All rights reserved.
//

#ifndef QuickSquare_hpp
#define QuickSquare_hpp

#include "Common.h"

class QuickSquare{
    
public :
    QuickSquare();
    QuickSquare(bool withSix);
    ~QuickSquare();
    
    void createSquare(const GLuint &shaderProgram);
    
    void bufferOfFourVertices();
    void bufferOfSixVertices();
    
    void drawSquare();
    
private:
    GLuint squareVAO;
    GLuint squareVBO;
    GLuint squareEBO;
    GLuint IndexCount = 0;
    
    float verticesOfSix[36] = {
        -0.6f,  0.6f, 0.0f, 1.0f, 0.0f, 0.0f, // Top-left // Vertex 1 (X, Y, Z) and Vertex 1: Red 
        0.6f,  0.6f, 0.0f, 0.0f, 1.0f, 0.0f, // Top-right // Vertex 2 (X, Y, Z) and Vertex 2: Green
        0.6f, -0.6f, 0.0f, 0.0f, 0.0f, 1.0f, // Bottom-right // Vertex 3 (X, Y, Z) and Vertex 3: Blue
        0.6f, -0.6f, 0.0f, 0.0f, 0.0f, 1.0f, // Bottom-right // Vertex 3 (X, Y, Z) and Vertex 3: Blue
        -0.6f, -0.6f, 0.0f, 1.0f, 1.0f, 1.0f, // Bottom-left // Vertex 3 (X, Y, Z) and Vertex 3: White
        -0.6f,  0.6f, 0.0f, 1.0f, 0.0f, 0.0f  // Top-left // Vertex 3 (X, Y, Z) and Vertex 3: Red
    };
    
    float verticesOfFour[24] = {
        -0.6f,  0.6f, 0.0f, 1.0f, 0.0f, 0.0f, // Top-left // Vertex 1 (X, Y, Z) and Vertex 1: Red 
        0.6f,  0.6f, 0.0f, 0.0f, 1.0f, 0.0f, // Top-right // Vertex 2 (X, Y, Z) and Vertex 2: Green
        0.6f, -0.6f, 0.0f, 0.0f, 0.0f, 1.0f, // Bottom-right // Vertex 3 (X, Y, Z) and Vertex 3: Blue
        -0.6f, -0.6f, 0.0f, 1.0f, 1.0f, 1.0f, // Bottom-left // Vertex 3 (X, Y, Z) and Vertex 3: White
    };
    
    bool withSix = false;
};

#endif /* QuickSquare_hpp */
