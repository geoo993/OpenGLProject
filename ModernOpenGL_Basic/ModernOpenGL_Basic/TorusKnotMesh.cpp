//
//  TorusKnotMesh.cpp
//  ModernOpenGL_Basic
//
//  Created by GEORGE QUENTIN on 27/02/2017.
//  Copyright © 2017 LEXI LABS. All rights reserved.
//

#include "TorusKnotMesh.h"


TorusKnotMesh::TorusKnotMesh(Vertex * vertices, const unsigned long int &numVertices, GLuint * indices, const unsigned long int & numIndices)
{
   
    m_Indices = numIndices;
    m_Indexes = indices;
    
    // create and use Vertex Array Object via binding
    glGenVertexArrays(1, &m_vertexArrayObject);
    glBindVertexArray(m_vertexArrayObject);
    
    std::vector<glm::vec3> positions;
    std::vector<glm::vec2> texCoords;
    std::vector<glm::vec3> normals;
    std::vector<unsigned int> indicess;
    
    positions.reserve(numVertices);
    texCoords.reserve(numVertices);
    normals.reserve(numVertices);
    indicess.reserve(numIndices);
    
    for(unsigned int i = 0; i < numVertices; i++)
    {
        positions.push_back(*vertices[i].GetPositions());
        texCoords.push_back(*vertices[i].GetTexCoordinates());
        normals.push_back(*vertices[i].GetNormals());
    }
    
    for(unsigned int i = 0; i < numIndices; i++)
    {
        indicess.push_back(indices[i]);
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
    
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_vertexArrayBuffers[INDEX_VB]);
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(indices[0]), &indices[0], GL_STATIC_DRAW );
    
    // unbind the Vertex Array Object
    glBindVertexArray(0);
    
}

void TorusKnotMesh::Draw(){
    
    //bind attribute array
    glBindVertexArray(m_vertexArrayObject);
   
    glDrawElements(GL_TRIANGLE_STRIP, m_Indices, GL_UNSIGNED_INT, 0);
  
    // Unbind Everything (NOTE: unbind the vertex array object first)
    glBindVertexArray(0);
    glBindBuffer( GL_ARRAY_BUFFER, 0);
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0); 
}

// Release memory on the GPU 
void TorusKnotMesh::Release(){
    
    glDeleteBuffers(NUM_BUFFERS, m_vertexArrayBuffers);
    glDeleteVertexArrays(1, &m_vertexArrayObject);
    
}


TorusKnotMesh::~TorusKnotMesh(){
    Release();
}
