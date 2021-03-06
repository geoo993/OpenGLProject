//
//  Torus.cpp
//  ModernOpenGL_Basic
//
//  Created by GEORGE QUENTIN on 27/02/2017.
//  Copyright © 2017 LEXI LABS. All rights reserved.
//

#include "TorusMesh.h"


TorusMesh::TorusMesh(Vertex * vertices, const unsigned int &numVertices)
{
    
    m_drawCount = numVertices;
    
    // create and use Vertex Array Object via binding
    glGenVertexArrays(1, &m_vertexArrayObject);
    glBindVertexArray(m_vertexArrayObject);
    
    std::vector<glm::vec3> positions;
    std::vector<glm::vec2> texCoords;
    std::vector<glm::vec3> normals;
    
    positions.reserve(numVertices);
    texCoords.reserve(numVertices);
    normals.reserve(numVertices);
    
    for(unsigned int i = 0; i < numVertices; i++)
    {
        positions.push_back(*vertices[i].GetPositions());
        texCoords.push_back(*vertices[i].GetTexCoordinates());
        normals.push_back(*vertices[i].GetNormals());
    }
  
    
    glGenBuffers(NUM_BUFFERS, m_vertexArrayBuffers);
    
    
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[POSITION_VB]);
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(positions[0]), &positions[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    
    //bind texture
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[TEXCOORD_VB]);
    glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(texCoords[0]), &texCoords[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
    
    //bind normals
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[NORMAL_VB]);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(normals[0]), &normals[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    // unbind the Vertex Array Object
    glBindVertexArray(0);
    
}

void TorusMesh::Draw(){
    
    //bind attribute array
    glBindVertexArray(m_vertexArrayObject);
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, m_drawCount);
    
    // Unbind Everything (NOTE: unbind the vertex array object first)
    glBindVertexArray(0);
    glBindBuffer( GL_ARRAY_BUFFER, 0);
}

// Release memory on the GPU 
void TorusMesh::Release(){
    
    glDeleteBuffers(NUM_BUFFERS, m_vertexArrayBuffers);
    glDeleteVertexArrays(1, &m_vertexArrayObject);
    
}


TorusMesh::~TorusMesh(){
    Release();
}
