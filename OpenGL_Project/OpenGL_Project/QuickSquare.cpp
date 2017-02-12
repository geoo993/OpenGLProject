//
//  QuickSquare.cpp
//  OpenGL_Project
//
//  Created by GEORGE QUENTIN on 11/02/2017.
//  Copyright © 2017 LEXI LABS. All rights reserved.
//

#include "QuickSquare.hpp"


QuickSquare::QuickSquare(){
    
}

QuickSquare::QuickSquare(bool withSix): withSix(withSix){
    
}

QuickSquare::~QuickSquare(){
    
    glDeleteVertexArrays(1, &squareVAO);
    
    glDeleteBuffers(1, &squareEBO);
    glDeleteBuffers(1, &squareVBO);
    
    
}

void QuickSquare::bufferOfFourVertices(){
    
    // Add VBO of the square using GL_ARRAY_BUFFER
    glBindBuffer(GL_ARRAY_BUFFER, squareVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesOfFour), verticesOfFour, GL_STATIC_DRAW);
    
    // Create an element buffor object array
    glGenBuffers(1, &squareEBO);
    
    GLuint elements[] = {
        0, 1, 2,
        2, 3, 0
    };
    
    // now using a EBO for the square using GL_ELEMENT_ARRAY_BUFFER
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, squareEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
}

void QuickSquare::bufferOfSixVertices(){
    // Add VBO of the square using GL_ARRAY_BUFFER
    glBindBuffer(GL_ARRAY_BUFFER, squareVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesOfSix), verticesOfSix, GL_STATIC_DRAW);
}

void QuickSquare::createSquare(const GLuint &shaderProgram){

    // Create Vertex Array Object
    glGenVertexArrays(1, &squareVAO);
    glBindVertexArray(squareVAO);
    
    // Create a Vertex Buffer Object and copy the vertex data to it
    glGenBuffers(1, &squareVBO);
    glBindVertexArray(squareVAO);
    
    
    if (withSix){
        bufferOfSixVertices();
        std::cout << "Using six vertices." << std::endl;
    }else{
        bufferOfFourVertices();
        std::cout << "Using four vertices." << std::endl;
    }

    // Specify the layout of the vertex data
    GLint posAttribute = glGetAttribLocation(shaderProgram, "vert_position");
    glEnableVertexAttribArray(posAttribute);
    glVertexAttribPointer(posAttribute, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
    
    GLint colorAttribute = glGetAttribLocation(shaderProgram, "color");
    glEnableVertexAttribArray(colorAttribute);
    glVertexAttribPointer(colorAttribute, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    
    //Use our shader program
    glUseProgram(shaderProgram);
    
}

void QuickSquare::drawSquare(){
    
    if (withSix){
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }else{
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
    
}
