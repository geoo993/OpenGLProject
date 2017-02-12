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
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteProgram(shaderProgram);

    glDeleteVertexArrays(1, &triangleVAO);
    glDeleteBuffers(1, &triangleVBO);

}

void QuickTriangle::createTriangle(){
    
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
    //glVertexAttribPointer(posAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glVertexAttribPointer(posAttribute, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
    
    //GLint colorAttribute = glGetUniformLocation(shaderProgram, "frag_color");
    //glUniform3f(colorAttribute, 1.0f, 0.0f, 0.0f);
    //glVertexAttribPointer(colorAttribute, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    
    GLint colorAttribute = glGetAttribLocation(shaderProgram, "color");
    glEnableVertexAttribArray(colorAttribute);
    glVertexAttribPointer(colorAttribute, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    
    
}


void QuickTriangle::drawTriangle(){
    glDrawArrays(GL_TRIANGLES, 0, 3);
}
