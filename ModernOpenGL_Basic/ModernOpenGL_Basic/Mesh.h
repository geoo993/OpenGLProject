//
//  Mesh.hpp
//  ModernOpenGL_Basic
//
//  Created by GEORGE QUENTIN on 25/02/2017.
//  Copyright © 2017 LEXI LABS. All rights reserved.
//

#ifndef Mesh_h
#define Mesh_h

#include "Common.h"

class Vertex{
    
public:
    Vertex(const glm::vec3 &pos, const glm::vec2 &texCoord){
        this->pos = pos;
        this->texCoord = texCoord;
    }
    
private:
    glm::vec3 pos;
    glm::vec2 texCoord;
};


class Mesh{
    
public:
    Mesh(Vertex * vertices, unsigned int numVertices);
    virtual ~Mesh();
    
    void Draw();
    void Release();

        
private:
    Mesh(const Mesh &other){}
    void operator=(const Mesh &other){}
    
    enum {
        POSITION_VB,
        TEXCOORD_VB,
        NORMAL_VB,
        
        NUM_BUFFERS
    };
    
    //this is the modern openGL way of refering to mesh data on the gpu, only officially indtrouced in openGL 3.0
    GLuint m_vertexArrayObject;
    GLuint m_vertexArrayBuffers[NUM_BUFFERS];
    
    
    //tell openGL how much of these vertex array object we want to draw
    unsigned int m_drawCount;
    
    
};

#endif /* Mesh_h */
