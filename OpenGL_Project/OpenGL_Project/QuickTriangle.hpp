//
//  QuickTriangle.hpp
//  OpenGL_Project
//
//  Created by GEORGE QUENTIN on 11/02/2017.
//  Copyright © 2017 LEXI LABS. All rights reserved.
//

#ifndef QuickTriangle_hpp
#define QuickTriangle_hpp

#include "Common.h"

class QuickTriangle{

public :
    QuickTriangle();
    ~QuickTriangle();
    
    void createTriangle(const GLuint &shaderProgram);
    void drawTriangle();
    
private:
    GLuint triangleVAO;
    GLuint triangleVBO;
};

#endif /* QuickTriangle_hpp */
