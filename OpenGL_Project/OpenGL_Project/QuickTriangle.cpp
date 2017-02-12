//
//  DrawTriangle.cpp
//  OpenGL_Project
//
//  Created by GEORGE QUENTIN on 11/02/2017.
//  Copyright © 2017 LEXI LABS. All rights reserved.
//

#include "QuickTriangle.hpp"

QuickTriangle::QuickTriangle(){
    
}

QuickTriangle::~QuickTriangle(){
    
    glDeleteVertexArrays(1, &triangleVAO);
    glDeleteBuffers(1, &triangleVBO);

}

void QuickTriangle::createTriangle(const GLuint &shaderProgram){

    glGenVertexArrays(1, &triangleVAO);
    glBindVertexArray(triangleVAO);
    
    glGenBuffers(1, &triangleVBO);//with defualt
    glBindVertexArray(triangleVAO);
    
    float vertices[] = {
        
        //positions and color 
        -0.8f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // Vertex 1 (X, Y, Z) and Vertex 1: Red 
        0.8f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, // Vertex 2 (X, Y, Z) and Vertex 2: Green
        0.0f, 0.8f, 0.0f, 0.0f, 0.0f, 1.0f  // Vertex 3 (X, Y, Z) and Vertex 3: Blue
        
        //        0.0f,  0.5f, 1.0f, 0.0f, 0.0f, // Vertex 1 (X, Y) and Vertex 1: Red
        //        0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // Vertex 2 (X, Y) and Vertex 2: Green
        //        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f  // Vertex 3 (X, Y) and Vertex 3: Blue
        
    };
    
    // Create a VBO for the triangle vertices
    glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    
    //    glEnableVertexAttribArray(0);
    //    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    // Specify the layout of the vertex data
    GLint posAttribute = glGetAttribLocation(shaderProgram, "vert_position");
    glEnableVertexAttribArray(posAttribute);
    //glVertexAttribPointer(posAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glVertexAttribPointer(posAttribute, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
    
    //GLint colorAttribute = glGetUniformLocation(shaderProgram, "frag_color");
    //glUniform3f(colorAttribute, 1.0f, 0.0f, 0.0f);
    //glVertexAttribPointer(colorAttribute, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    
    GLint colorAttribute = glGetAttribLocation(shaderProgram, "color");
    glEnableVertexAttribArray(colorAttribute);
    glVertexAttribPointer(colorAttribute, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    
    //Use our shader program
    glUseProgram(shaderProgram);
    
}


void QuickTriangle::drawTriangle(){
    
    glDrawArrays(GL_TRIANGLES, 0, 3);
}
