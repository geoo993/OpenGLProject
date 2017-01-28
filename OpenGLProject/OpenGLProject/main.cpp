//
//  main.cpp
//  OpenGLProject
//
//  Created by GEORGE QUENTIN on 26/01/2017.
//  Copyright © 2017 GEORGE QUENTIN. All rights reserved.
//


#include <iostream>

//Mac OpenGL
//#include <OpenGL/gl.h>
//#include <OpenGl/glu.h>

//Mac GLUT
//#include <GLUT/glut.h>

//GLEW
#include <GL/glew.h>

//freeglut
//#include <GL/freeglut.h>

//GLM
#include <glm/glm.hpp>

//Freestyle
//#include <freetype2/freetype/freetype.h>

//FreeImage
//#include <FreeImage.h>

//GLFW
#include <GLFW/glfw3.h>

//Assimp
//#include <assimp/scene.h>

//#include <stdlib.h>
//#include <stdio.h>


//void render(void){
//    
//    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
//    glClear(GL_COLOR_BUFFER_BIT);
//    
//    glBegin(GL_TRIANGLES);
//    {
//        glColor3f(1.0, 0.0, 0.0);
//        glVertex2f(0, 0.5f);
//        glColor3f(0.0, 1.0, 0.0);
//        glVertex2f(-0.05f, -0.5f);
//        glColor3f(0.0, 0.0, 1.0);
//        glVertex2f(0.5f, -0.5f);
//    }
//    glEnd();
//}
//
//int main(int argc, const char * argv[]) {
//    
//    GLFWwindow * window;
//    
//    if(glfwInit()){
//        return -1;
//    }
//    
//    window = glfwCreateWindow(800, 600, "OpenGL Project", NULL, NULL);
//    if(!window){
//        glfwTerminate();
//        exit(EXIT_FAILURE);
//    }
//    
//    if(!glewInit()){
//        return -1;
//    }
//    
//    glfwMakeContextCurrent(window);
//    
//    while (!glfwWindowShouldClose(window)) {
//        render();
//        
//        glfwSwapBuffers(window);
//        glfwPollEvents();
//    }
//    
//    glfwDestroyWindow(window);
//    glfwTerminate();
//    exit(EXIT_SUCCESS);
//    
//    return 0;
//}


static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

//int main(void)
int main(int argc, const char * argv[]) {
    //    std::cout << "Hello, World!\n";
    
    GLFWwindow* window;
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);
    window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    glfwSetKeyCallback(window, key_callback);
    while (!glfwWindowShouldClose(window))
    {
        float ratio;
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float) height;
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glRotatef((float) glfwGetTime() * 50.f, 0.f, 0.f, 1.f);
        glBegin(GL_TRIANGLES);
        glColor3f(1.f, 0.f, 0.f);
        glVertex3f(-0.6f, -0.4f, 0.f);
        glColor3f(0.f, 1.f, 0.f);
        glVertex3f(0.6f, -0.4f, 0.f);
        glColor3f(0.f, 0.f, 1.f);
        glVertex3f(0.f, 0.6f, 0.f);
        glEnd();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
    
    return 0;
}
 
 
