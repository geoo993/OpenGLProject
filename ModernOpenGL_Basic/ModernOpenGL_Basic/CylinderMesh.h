//
//  CylinderMesh.hpp
//  ModernOpenGL_Basic
//
//  Created by GEORGE QUENTIN on 02/03/2017.
//  Copyright © 2017 LEXI LABS. All rights reserved.
//

#ifndef CylinderMesh_h
#define CylinderMesh_h

#include "Common.h"
#include "Vertex.h"

class CylinderMesh{
    
public:
    
    CylinderMesh(Vertex * vertices, const GLuint &numVertices, const GLuint &topTriangleFanCount, const GLuint &bottomTriangleFanCount);
    
    virtual ~CylinderMesh();
    
    void Draw();
    void Release();
    
    
private:
    CylinderMesh(const CylinderMesh &other){}
    void operator=(const CylinderMesh &other){}
    
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
    unsigned int m_topTriangleFanCount;
    unsigned int m_bottomTriangleFanCount;
    
    
};

#endif /* CylinderMesh_h */
