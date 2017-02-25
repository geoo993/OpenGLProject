//
//  Mesh.cpp
//  ModernOpenGL_Basic
//
//  Created by GEORGE QUENTIN on 25/02/2017.
//  Copyright © 2017 LEXI LABS. All rights reserved.
//

#include "Mesh.h"


Mesh::Mesh(Vertex * vertices, unsigned int numVertices){
    
    m_drawCount = numVertices;
    std::vector<glm::vec3> positions;
    std::vector<glm::vec2> texCoords;
    //std::vector<glm::vec3> normals;
    
    positions.reserve(numVertices);
    texCoords.reserve(numVertices);
    //normals.reserve(numVertices);
    
    // create and use Vertex Array Object via binding
    glGenVertexArrays(1, &m_vertexArrayObject);
    glBindVertexArray(m_vertexArrayObject);
    
    //the way openGL refers to data in the gpu is with Buffer Objects, which is just any block of data in the gpu
    //so to handle data in openGL, we need Vertex Buffer Objects
    //now we get data on gpu by genBuffer and bind
    glGenBuffers(NUM_BUFFERS, m_vertexArrayBuffers);
    //this will interpret the data as an array, so open gl will think that it is an array
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[POSITION_VB]);
    
    //now we want to put all our vertices in the array, so we are pushing our vertex data in the buffer array
    //we are sending the buffer array first, then how big the buffer array of our vertices is 
    //the reason we are doing numVertices * sizeof(vertices[0]) instead of sizeof(vertices), is jut for convention of working out the size of an array, no difference.
    //next in the third parameter we pass in the vertices array it self, and finaly in the forth parameter we want to give the draw hint which is GL_STATIC_DRAW, this is the hint of where openGl should put the data in the graphics card, and static draw means openGL is going to put the data is a safe area we it is not expecting it to be changes, as we specified that we are not planning to modify the data as it is (STATIC). 
    glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(vertices[0]), vertices, GL_STATIC_DRAW);
    
    
    //now we tell openGL how to interpret this data, once it is trying to draw it, so we use vertex attribute
    //Specify the layout of the vertex data
    glEnableVertexAttribArray(0);// first attribute in the vertex shader which is inPosition
    
    //now we tell openGL how to read that attribute array, we tell it how to read an attribute from the vertex shader
    //in the first parameter we tell it which attribute to look at which is zero
    //then we need to tell openGL how many pieces of data are in this attribute, so in this case 3, were there is three pieces of data in our position (vec3)
    //then we need to tell openGL what type of data these three pieces of data are, which in this case is CL_FLOAT specified in glm vec3 
    //then we need t tell openGL is we want to normalise it, but its best to set to GL_FALSE
    //now in the fith parameter, our data is going to be a continues attribute array, so we set it to zero
    //the last specifies how much it needs to start from the beginning to find the first attribute, so it going to be zero because this is the first attribute 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    
    // unbind the Vertex Array Object
    glBindVertexArray(0);
    
}


void Mesh::Draw(){
    
    //bind attribute array
    glBindVertexArray(m_vertexArrayObject);
    //m_texture.Bind();
    //glDrawElements(GL_TRIANGLES, m_numTriangles*3, GL_UNSIGNED_INT, 0);
    
    
    //openGL need a little bit more information about how to draw, 
    //first is the mode, which in this case is GL_TRIANGLES. this mode section is basically triangles and variance of triangles or lines, or points
    //then you have to specify where you want to start to draw, which is at 0, but you can specify where to start using values, and ensure that the value is within the bound of the vertex array buffer
    //finally how much you want to read or how far you want to read, you can specify how much you want opnGL to draw your triangles or mesh, in this case how many vertices to draw, whcih will be taken from the drawcount
    glDrawArrays(GL_TRIANGLES, 0, m_drawCount);
    
    
}

// Release memory on the GPU 
void Mesh::Release(){
    //m_texture.Release();
    glDeleteVertexArrays(1, &m_vertexArrayObject);
    //m_vbo.Release();
    
    
    // unbind the Vertex Array Object
    glBindVertexArray(0);
}


Mesh::~Mesh(){
    Release();
}
