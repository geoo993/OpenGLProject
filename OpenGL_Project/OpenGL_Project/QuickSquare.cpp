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
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteProgram(shaderProgram);
    
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

void QuickSquare::createSquare(){

    const char* vertexShaderSource =
    "#version 400\n"
    "in vec3 vert_position;"
    "in vec3 color;"
    "out vec3 frag_color;"//from frag shader
    "void main() {"
    "  frag_color = color;"
    "  gl_Position = vec4 (vert_position, 1.0);"
    "}";
    
    const char* fragmentShaderSource =
    "#version 400\n"
    //"uniform vec3 uni_color;"
    "in vec3 frag_color;"
    "out vec4 out_color;"
    "void main() {"
    "  out_color = vec4(frag_color, 1.0);"
    "}";
    
    
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

    
    // Create and compile the vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);
    
    GLint status;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
    
    char buffer[512];
    glGetShaderInfoLog(vertexShader, 512, NULL, buffer);
    
    // Create and compile the fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);
    
    // Link the vertex and fragment shader into a shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, fragmentShader);
    glAttachShader(shaderProgram, vertexShader);
    glBindFragDataLocation(shaderProgram, 0, "out_color");
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);
    
    // Specify the layout of the vertex data
    GLint posAttribute = glGetAttribLocation(shaderProgram, "vert_position");
    glEnableVertexAttribArray(posAttribute);
    glVertexAttribPointer(posAttribute, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
    
    GLint colorAttribute = glGetAttribLocation(shaderProgram, "color");
    glEnableVertexAttribArray(colorAttribute);
    glVertexAttribPointer(colorAttribute, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    
}

void QuickSquare::drawSquare(){
    
    if (withSix){
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }else{
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
    
}
