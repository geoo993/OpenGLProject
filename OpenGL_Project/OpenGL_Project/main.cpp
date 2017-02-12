//
//  main.cpp
//  OpenGL_Project
//
//  Created by GEORGE QUENTIN on 11/02/2017.
//  Copyright © 2017 LEXI LABS. All rights reserved.
//
#include "Common.h"
#include "QuickTriangle.hpp"
#include "QuickSquare.hpp"
#include "QuickCube.hpp"
#include "QuickTetrahedron.hpp"

int keyCode = -1; 

//degrees per seconds
float xRotation = 0.0f;
float yRotation = 0.0f;
float depth = - 5.0f;

bool autoRotate = false;

float size = 1.0f;

std::string shape = "tetrahedron";


static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
   
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    
    if (key == GLFW_KEY_UP && action == GLFW_PRESS){
        depth += 1.0f;
    }
    
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS){
        depth -= 1.0f;
    }
    
    //std::cout << "Key pressed with key: " << key << " and with action: " << action << std::endl;
    
}

int px = 0;
int py = 0;

void cursor_callback(GLFWwindow *window, double xpos, double ypos)
{
    
    int x = xpos - px;
    int y = ypos - py;
    
    xRotation += x;
    yRotation += y;
    
    px = xpos;
    py = ypos;
    
    
    //std::cout << "Mouse cursor   x: " << xRotate << " y: " << yRotate << std::endl;
}


GLFWwindow* initialiseWindow(const int width, const int height)
{
    // start GL context and O/S window using the GLFW helper library
    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        exit(EXIT_FAILURE);
        return nullptr;
    } 
    
    
    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
    if(shape == "triangle" || shape == "square"){
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//We don't want the old OpenGL and want the core profile
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    }
    
    // Open a window and create its OpenGL context
    GLFWwindow* window = glfwCreateWindow(width, height, "OpenGL Window", nullptr, nullptr);
    
    if(window == nullptr)
    {
        fprintf(stderr, "ERROR: could not open window with GLFW3\n");
        glfwTerminate();
        return nullptr;
    }
    
    glfwSetErrorCallback(error_callback);
    
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, keyboard_callback);
    glfwSetCursorPosCallback(window, cursor_callback);

    
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
    
    glEnable(GL_DEPTH_TEST); // enable depth-testing, //enables you to draw things in correct order
    glEnable(GL_CULL_FACE);//only front facinf poligons are rendered, forn facing have a clock wise or counter clock wise order
    //glDisable(GL_CULL_FACE);
    
    //glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
    glDepthFunc(GL_LEQUAL);
    
    //glCullFace(GL_BACK);
    
    return window;
}

void screenProjection( const int &width, const int &height){
    
    // compute the aspect ratio
    // this is used to prevent the picture from distorting when
    // the window is resized
    float aspect = (GLfloat)width / (GLfloat)height;
    
    // Draw stuff
    glMatrixMode(GL_PROJECTION_MATRIX);
    glLoadIdentity();//sets the model view matrix back to its initial state, which puts the camera directly in front of the world.
    
//    if (windowWidth >= windowHeight) {
//        // aspect >= 1, set the height from -1 to 1, with larger width
//        //gluOrtho2D(-1.0 * aspect, 1.0 * aspect, -1.0, 1.0);
//        glOrtho(-1.0f * aspect, 1.0f * aspect, -1.0f, 1.0f, 1.0f, -1.0f);
//    } else {
//        // aspect < 1, set the width to -1 to 1, with larger height
//        //gluOrtho2D(-1.0, 1.0, -1.0 / aspect, 1.0 / aspect);
//        glOrtho(-1.0f, 1.0f, -1.0 / aspect, 1.0 / aspect, 1.0f, -1.0f);
//    }

    gluPerspective( 45, aspect, 0.2f, 100.0f );
    
    glMatrixMode(GL_MODELVIEW_MATRIX);
    
}

void controlShape(){
    
    glTranslatef( 0.0f, 0.0f, depth);// move the rest of the world back away from the camera
    
    if (autoRotate == true){
        //continously to rotate cube
        static float rotation = 0;
        glRotatef(rotation, 0.0f, 1.0f, 0.0f);
        rotation += 1;
    }else{
        glRotatef(xRotation, 0.0, 1.0f, 0.0f);
        glRotatef(yRotation, 1.0f, 0.0f, 0.0f);
    }
}

void displayWindow( GLFWwindow* window )
{
    QuickTriangle triang;
    QuickSquare square(false);
    QuickCube cube;
    QuickTetrahedron tetrahedron;
    
    if(shape == "triangle"){
        triang.createTriangle();
        std::cout << "Triangle";
    }else if (shape == "square"){
        square.createSquare();
        std::cout << "Square";
    }else if (shape == "cube"){
        cube.drawVertices(size);
        std::cout << "Cube";
    }else if (shape == "tetrahedron"){
        tetrahedron.drawVertices(size);
        std::cout << "Tetrahedron";
    }
    
    
    while(!glfwWindowShouldClose(window))
    {
        // Scale to window size
        GLint windowWidth, windowHeight;
        glfwGetWindowSize(window, &windowWidth, &windowHeight);
        glfwGetFramebufferSize(window, &windowWidth, &windowHeight);
        
        if (windowHeight == 0) windowHeight = 1;                // To prevent divide by 0
        glViewport(0, 0, windowWidth, windowHeight);
        
        // Clear the screen
        glClearColor(0.4f, 0.4f, 0.4f, 1.0f); 
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        screenProjection(windowWidth, windowHeight);
        
        controlShape();
        
        if(shape == "triangle"){
            triang.drawTriangle();
        }else if (shape == "square"){
            square.drawSquare();
        }else if (shape == "cube"){
            cube.drawCube();
        }else if (shape == "tetrahedron"){
            tetrahedron.drawTetrahedron();
        }
        
        // Update Screen
        glfwSwapBuffers(window);
        
        // Check for any input, or window movement
        glfwPollEvents();
    }
  
}

int main(int argc, const char * argv[]) {
    
    GLFWwindow* window = initialiseWindow(760, 620);
    if( window != nullptr )
    {
        displayWindow( window );
    }
    
    // close GL context and any other GLFW resources
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
    
    return 0;
}

