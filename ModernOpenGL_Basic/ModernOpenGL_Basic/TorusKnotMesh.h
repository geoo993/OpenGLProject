//
//  TorusKnotMesh.hpp
//  ModernOpenGL_Basic
//
//  Created by GEORGE QUENTIN on 27/02/2017.
//  Copyright © 2017 LEXI LABS. All rights reserved.
//

#ifndef TorusKnotMesh_h
#define TorusKnotMesh_h

#include "Common.h"
#include "Vertex.h"


class TorusKnotMesh{
    
public:
    
    TorusKnotMesh(Vertex * vertices, const unsigned int &numVertices, GLuint * indices, const unsigned int & numIndices);
    
    virtual ~TorusKnotMesh();
    
    void Draw();
    void Release();
    
    
private:
    TorusKnotMesh(const TorusKnotMesh &other){}
    void operator=(const TorusKnotMesh &other){}
    
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
    
    GLuint * m_Indexes;
    unsigned int m_Indices;
    
    
};


#endif /* TorusKnotMesh_h */
