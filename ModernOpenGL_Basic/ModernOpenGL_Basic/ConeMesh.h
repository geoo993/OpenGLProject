//
//  ConeMesh.hpp
//  ModernOpenGL_Basic
//
//  Created by GEORGE QUENTIN on 02/03/2017.
//  Copyright © 2017 LEXI LABS. All rights reserved.
//

#ifndef ConeMesh_h
#define ConeMesh_h

#include "Common.h"
#include "Vertex.h"

class ConeMesh{
    
public:
    
    ConeMesh(Vertex * vertices, const GLuint &numVertices);
    
    virtual ~ConeMesh();
    
    void Draw();
    void Release();
    
    
private:
    ConeMesh(const ConeMesh &other){}
    void operator=(const ConeMesh &other){}
    
    enum {
        POSITION_VB,
        TEXCOORD_VB,
        NORMAL_VB,
        INDEX_VB,
        
        NUM_BUFFERS
    };
    
    //this is the modern openGL way of refering to mesh data on the gpu, only officially indtrouced in openGL 3.0
    GLuint m_vertexArrayObject;
    GLuint m_vertexArrayBuffers[NUM_BUFFERS];
    
    unsigned int m_drawCount;
    
    
};

#endif /* ConeMesh_h */
