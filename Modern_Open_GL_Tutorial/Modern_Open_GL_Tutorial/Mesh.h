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
#include "GameObject.h"
#include "Vertex.h"
#include "Texture.h"
#include "Transform.h"

class Mesh{
    
public:
    Mesh(){}
    virtual ~Mesh();
    
    void Create(Vertex * vertices, const unsigned int &numVertices);
    
    void Create(Vertex * uniqueVertices, const unsigned int &numUniqueVertices, unsigned int * indices, const unsigned int & numIndices);
    void Create(const std::string &fileName);
    void Draw();
    void Release();

    Transform transform;
    
private:
    Mesh(const Mesh &other){}
    void operator=(const Mesh &other){}
    
    void GenerateAndBindMeshData(const IndexedModel & model, const bool & withIndices);
    
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
    
    //tell openGL how much of these vertex array object we want to draw
    unsigned int m_drawCount;
    
    bool usingIndices;
    
};

#endif /* Mesh_h */
