//
//  main.cpp
//  OpenGL_Project
//
//  Created by GEORGE QUENTIN on 11/02/2017.
//  Copyright © 2017 LEXI LABS. All rights reserved.
//

#include <iostream>

#define GLEW_STATIC

//Mac OpenGL
//#include <OpenGL/gl.h>
//#include <OpenGl/glu.h>
//#include <OpenGL/OpenGL.h>
//#include <OpenGL/gl3.h>

//Mac GLUT
//#include <GLUT/glut.h>

//GLEW
#include <GL/glew.h>//include GLEW and new version of GL on Windows

//freeglut
//#include <GL/freeglut.h>

//GLM
//#include <glm/glm.hpp>

//GLFW
#include <GLFW/glfw3.h>// GLFW helper library


//#include <stdlib.h>
//#include <stdio.h>

//#include "sys/time.h"
//#include <thread>


int width = 1024;
int height = 768;

GLuint triangleVAO;
GLuint shaderProgram;

void drawTriangle(){
    
    GLuint triangleVBO[2];
    
    glGenVertexArrays(1, &triangleVAO);
    
    glGenBuffers(2, &triangleVBO[0]);
    
    glBindVertexArray(triangleVAO);
    
    
    //triangle points
    float points[] = {
        -1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    };
    
    // Create a VBO for the triangle vertices
    glBindBuffer(GL_ARRAY_BUFFER, triangleVBO[0]);
    
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), points, GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    
    
    //triangle colours
    float colors[] = {
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f
    };
    // Create a VBO for the triangle colours
    glBindBuffer(GL_ARRAY_BUFFER, triangleVBO[1]);
    
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), colors, GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(1);
    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    
    
    
    //add shaders
    const char* vertexShaderSource =
    "#version 400\n"
    "in vec3 vp;"
    "void main() {"
    "  gl_Position = vec4 (vp, 1.0);"
    "}";
    
    const char* fragmentShaderSource =
    "#version 400\n"
    "out vec4 frag_colour;"
    "void main() {"
    "  frag_colour = vec4(1.0, 1.0, 1.0, 1.0);"
    "}";
    
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    
    glCompileShader(vertexShader);
    
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    
    glCompileShader(fragmentShader);
    
    
    shaderProgram = glCreateProgram();
    
    glAttachShader(shaderProgram, fragmentShader);
    
    glAttachShader(shaderProgram, vertexShader);
    
    glLinkProgram(shaderProgram);
    
}



static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";

    // start GL context and O/S window using the GLFW helper library
    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        exit(EXIT_FAILURE);
        return -1;
    } 
    
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//We don't want the old OpenGL and want the core profile
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    
    GLFWwindow* window = glfwCreateWindow(height, height, "OpenGL Window", nullptr, nullptr);
    
    if (!window) {
        fprintf(stderr, "ERROR: could not open window with GLFW3\n");
        glfwTerminate();
        return -1;
    }
    
    glfwSetErrorCallback(error_callback);
    
    glfwMakeContextCurrent(window);
    
    // start GLEW extension handler
    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK)
        throw std::runtime_error("glewInit failed");
    
    if(!GLEW_VERSION_3_2)
        throw std::runtime_error("OpenGL 3.2 API is not available.");
    
    // get version info
    const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
    const GLubyte* version = glGetString(GL_VERSION); // version as a string
    printf("Renderer: %s\n", renderer);
    printf("OpenGL version supported %s\n", version);
    
      
    
    drawTriangle();
    
    glEnable(GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
    
    while(!glfwWindowShouldClose(window))
    {
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0,0,width,height);
        glClearColor(.4,.4,.4,1); 
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // tell GL to only draw onto a pixel if the shape is closer to the viewer
    
        glLoadIdentity();	
 
        // Bind the VAO
        glBindVertexArray(triangleVAO);
        
        //use shader program
        glUseProgram(shaderProgram);
        
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        glfwSwapBuffers(window);
        
        //if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        //    glfwSetWindowShouldClose(window, GL_TRUE);
        glfwPollEvents();
    }
    
    // close GL context and any other GLFW resources
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
    
    
    return 0;
}
