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
    Vertex(const glm::vec3 &position, const glm::vec2 &texCoord, const glm::vec3 &normal){
        this->position = position;
        this->texCoord = texCoord;
        this->normal = normal;
    }
    
    inline glm::vec3* GetPositions() { return &position; }
    inline glm::vec2* GetTexCoordinates() { return &texCoord; }
    inline glm::vec3* GetNormals() { return &normal; }
    
private:
    glm::vec3 position;
    glm::vec2 texCoord;
    glm::vec3 normal;
    
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
    
    // Define the vertex indices for the cube.
    GLuint indices[24] = {
        0, 1, 2, 3,                 // Front face
        7, 4, 5, 6,                 // Back face
        6, 5, 2, 1,                 // Left face
        7, 0, 3, 4,                 // Right face
        7, 6, 1, 0,                 // Top face
        3, 2, 5, 4,                 // Bottom face
    };
    
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
