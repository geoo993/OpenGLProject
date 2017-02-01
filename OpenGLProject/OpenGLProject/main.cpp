//
//  main.cpp
//  OpenGLProject
//
//  Created by GEORGE QUENTIN on 26/01/2017.
//  Copyright © 2017 GEORGE QUENTIN. All rights reserved.
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

//Freestyle
//#include <freetype2/freetype/freetype.h>

//FreeImage
//#include <FreeImage.h>

//GLFW
#include <GLFW/glfw3.h>// GLFW helper library

//Assimp
//#include <assimp/scene.h>

//#include <stdlib.h>
//#include <stdio.h>

//#include "sys/time.h"
//#include <thread>


int width = 1024;
int height = 768;

GLuint triangleVAO = 0;
GLuint shaderProgram;

void addShaders(){
    
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

void drawTriangle(){
    
    float points[] = {
        0.0f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f
    };
    
    GLuint triangleVBO = 0;
    
    glGenBuffers(1, &triangleVBO);
    
    glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
    
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), points, GL_STATIC_DRAW);
    
    glGenVertexArrays(1, &triangleVAO);
    
    glBindVertexArray(triangleVAO);
    
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    
    
    addShaders();
    
}



static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

int main(int argc, char **argv)
{
    // start GL context and O/S window using the GLFW helper library
    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        exit(EXIT_FAILURE);
        return -1;
    } 

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    
    GLFWwindow* window = glfwCreateWindow(height, height, "OpenGL Window", nullptr, nullptr); // Windowed
    //GLFWwindow* window = glfwCreateWindow(height, height, "OpenGL", glfwGetPrimaryMonitor(), nullptr); // Fullscreen
   
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
    
    // tell GL to only draw onto a pixel if the shape is closer to the viewer
    glEnable(GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
    
    
    while(!glfwWindowShouldClose(window))
    {
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0,0,width,height);
        glClearColor(.2,.8,.8,1);                  ////<-- CLEAR WINDOW CONTENTS
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Bind the VAO
        glBindVertexArray(triangleVAO);
        
        //use shader program
        glUseProgram(shaderProgram);
        
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
