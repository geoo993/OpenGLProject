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
#include "QuickTrianglesGube.hpp"

#define SCREEN_WIDTH 720
#define SCREEN_HEIGHT 680

int keyCode = -1; 

//degrees per seconds
float xRotation = 0.0f;
float yRotation = 0.0f;
float depth = - 5.0f;

bool autoRotate = true;

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
    if(shape == "triangle" || shape == "square" || shape == "trianglesCube" || shape == "newCube")
    {
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
   
    if(shape == "triangle" || shape == "cube" || shape == "tetrahedron" || shape == "trianglesCube" || shape == "newCube"){
        glEnable(GL_CULL_FACE);//only front facinf poligons are rendered, forn facing have a clock wise or counter clock wise order
    }else if (shape == "square"){
        glDisable(GL_CULL_FACE);
    }
    
    //glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
    //glDepthFunc(GL_LEQUAL);
    
    //glCullFace(GL_BACK);
    
    
    return window;
}

GLuint addShader(){
    
    const char* vertexShaderSource =
    "#version 400\n"
    "layout(location = 0) in vec3 vert_position;"
    "layout(location = 1) in vec3 color;"
    "out vec3 frag_color;"//from frag shader
    "uniform mat4 MVP;\n"
    "void main() {"
    "  gl_Position = vec4 (vert_position, 1.0);"
    "  frag_color = color;"
    "}";
    //MVP *
    const char* fragmentShaderSource =
    "#version 400\n"
    //"uniform vec3 uni_color;"
    "in vec3 frag_color;"
    "out vec4 out_color;"
    "void main() {"
    "  out_color = vec4(frag_color, 1.0);"
    "}";
    
    // Create and compile the vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);
    
    // Check Vertex Shader
    GLint status = GL_FALSE;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
    
    int infoLogLength;
    glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &infoLogLength);
    
    if ( infoLogLength > 0 ){
        std::vector<char> vertexShaderErrorMessage(infoLogLength+1);
        glGetShaderInfoLog(vertexShader, infoLogLength, nullptr, &vertexShaderErrorMessage[0]);
        printf("%s\n", &vertexShaderErrorMessage[0]);
    }
    
    // Create and compile the fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);
    
    // Check Fragment Shader
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
    glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &infoLogLength);
    if ( infoLogLength > 0 ){
        std::vector<char> fragmentShaderErrorMessage(infoLogLength+1);
        glGetShaderInfoLog(fragmentShader, infoLogLength, nullptr, &fragmentShaderErrorMessage[0]);
        printf("%s\n", &fragmentShaderErrorMessage[0]);
    }
    
    //Link the vertex and fragment shader into a shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glBindFragDataLocation(shaderProgram, 0, "out_color");
    glLinkProgram(shaderProgram);
    
    // Check the shader program
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &status);
    glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &infoLogLength);
    if ( infoLogLength > 0 ){
        std::vector<char> shaderProgramErrorMessage(infoLogLength+1);
        glGetProgramInfoLog(shaderProgram, infoLogLength, nullptr, &shaderProgramErrorMessage[0]);
        printf("%s\n", &shaderProgramErrorMessage[0]);
    }
    
    glDetachShader(shaderProgram, vertexShader);
    glDetachShader(shaderProgram, fragmentShader);
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
   
    
    return shaderProgram;
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


void mvp(const float &w, const float &h, const GLuint &programID){
    
    glViewport(0.0f, 0.0f, w, h);
    
    // compute the aspect ratio
    // this is used to prevent the picture from distorting when
    // the window is resized
    float aspect = (float)w / (float)h;
    
    if(shape == "triangle" || shape == "square" || shape == "trianglesCube" || shape == "newCube")
    {
        // Get a handle for our "MVP" uniform
        GLuint MatrixID = glGetUniformLocation(programID, "MVP");
        
        // Projection matrix : 45∞ Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
        glm::mat4 Projection = glm::perspective(glm::radians(45.0f), aspect, 0.2f, 100.0f);
        // Or, for an ortho camera :
        //glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates
        
        // Camera matrix
        glm::mat4 View       = glm::lookAt(
                                           glm::vec3(4,3,-3), // Camera is at (4,3,-3), in World Space
                                           glm::vec3(0,0,0), // and looks at the origin
                                           glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
                                           );
        // Model matrix : an identity matrix (model will be at the origin)
        glm::mat4 Model      = glm::mat4(1.0f);
        // Our ModelViewProjection : multiplication of our 3 matrices
        glm::mat4 MVP        = Projection * View * Model; // Remember, matrix multiplication is the other way around
        
        // Send our transformation to the currently bound shader, 
        // in the "MVP" uniform
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        
        
    }else{
        
        // Draw stuff
        glMatrixMode(GL_PROJECTION_MATRIX);
        //glMatrixMode(GL_PROJECTION);// projection matrix defines the properties of the camera that views the object in the world coordinates frame.
        glLoadIdentity();//sets the model view matrix back to its initial state, which puts the camera directly in front of the world.
        
//        if (windowWidth >= windowHeight) {
//            glOrtho(-1.0f * aspect, 1.0f * aspect, -1.0f, 1.0f, 1.0f, -1.0f);
//        } else {
//            glOrtho(-1.0f, 1.0f, -1.0 / aspect, 1.0 / aspect, 1.0f, -1.0f);
//        }
        //glOrtho(0.0f, SCREEN_WIDTH, 0.0f, SCREEN_HEIGHT, 0.0f, 10.0f);// essentially set coordinate system
        
        gluPerspective( 45, 1.0f * aspect, 0.2f, 100.0f );
        
        glMatrixMode(GL_MODELVIEW_MATRIX);// (default matrix mode) modelview matrix defines how your objects are transformed (meaning translation, rotation, and scaling) in your world 
        //glLoadIdentity(); // same as above
        
    }
    
    
}


void displayWindow( GLFWwindow* window )
{
    GLuint programID;
    QuickTriangle triang;
    QuickSquare square(true);
    QuickCube cube;
    QuickTetrahedron tetrahedron;
    QuickTrianglesGube trianglesCube;
    
    if (shape == "triangle" || shape == "square" || shape == "trianglesCube" || shape == "newCube")
    {
        programID = addShader();
        std::cout << "adding shader with modern open gl." << std::endl;
    }
    
    if(shape == "triangle"){
        triang.createTriangle(programID);
        std::cout << "Triangle";
    }else if (shape == "square"){
        square.createSquare(programID);
        std::cout << "Square";
    }else if (shape == "cube"){
        cube.drawVertices(size);
        std::cout << "Cube";
    }else if (shape == "tetrahedron"){
        tetrahedron.drawVertices(size);
        std::cout << "Tetrahedron";
    }else if (shape == "trianglesCube"){
        trianglesCube.drawVertices(size);
        std::cout << "Triangles Cude";
    }

    
    // Scale to window size
    GLint windowWidth, windowHeight;
    glfwGetWindowSize(window, &windowWidth, &windowHeight);
    glfwGetFramebufferSize(window, &windowWidth, &windowHeight);
    if (windowHeight == 0) windowHeight = 1; // To prevent divide by 0
    

    while(!glfwWindowShouldClose(window))
    {
        // Clear the screen
        glClearColor(0.4f, 0.4f, 0.4f, 1.0f); 
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        mvp(windowWidth,windowHeight, programID);
        
        controlShape();
        
        
        if(shape == "triangle"){
            triang.drawTriangle();
        }else if (shape == "square"){
            square.drawSquare();
        }else if (shape == "cube"){
            cube.drawCube();
        }else if (shape == "tetrahedron"){
            tetrahedron.drawTetrahedron();
        }else if (shape == "trianglesCube"){
            trianglesCube.drawCube(programID);
        }else if (shape == "newCube"){
            
        }
        
       
        // Update Screen
        glfwSwapBuffers(window);
        
        // Check for any input, or window movement
        glfwPollEvents();
    }
  
}

int main(int argc, const char * argv[]) {
    
    GLFWwindow* window = initialiseWindow(SCREEN_WIDTH, SCREEN_HEIGHT);
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

