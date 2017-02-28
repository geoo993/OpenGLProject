//
//  Torus.hpp
//  ModernOpenGL_Basic
//
//  Created by GEORGE QUENTIN on 27/02/2017.
//  Copyright © 2017 LEXI LABS. All rights reserved.
//

#ifndef TorusMesh_h
#define TorusMesh_h

#include "Common.h"
#include "Vertex.h"


class TorusMesh{
    
public:
    
    TorusMesh(Vertex * vertices, const unsigned int &numVertices);
    
    virtual ~TorusMesh();
    
    void Draw();
    void Release();
    
    
private:
    TorusMesh(const TorusMesh &other){}
    void operator=(const TorusMesh &other){}
    
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



#endif /* TorusMesh_h */
