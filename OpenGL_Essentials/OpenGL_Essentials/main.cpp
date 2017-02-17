//
//  main.cpp
//  OpenGL_Essentials
//
//  Created by GEORGE QUENTIN on 15/02/2017.
//  Copyright © 2017 LEXI LABS. All rights reserved.
//

#include <iostream>

#define GLFW_INCLUDE_GL3  /* don't drag in legacy GL headers. */
#define GLFW_NO_GLU       /* don't drag in the old GLU lib - unless you must. */

#define GLEW_STATIC

//GLEW
#include <GL/glew.h>//include GLEW and new version of GL on Windows

//GLFW
#define GLFW_DLL
#include <GLFW/glfw3.h>// GLFW helper library

#include <FreeImage.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <OpenGL/gl.h>

#include <stdexcept>
#include <cstdlib>

#include "gl_data.hpp"
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <cstdio>

#define SCREEN_WIDTH 720
#define SCREEN_HEIGHT 680

std::string texturepath = "/Users/GeorgeQuentin/Dev/OpenGL/OpenGLProject/OpenGL_Essentials/OpenGL_Essentials/resources/wooden-crate.jpg";
std::string vertpath = "/Users/GeorgeQuentin/Dev/OpenGL/OpenGLProject/OpenGL_Essentials/OpenGL_Essentials/resources/vertex-shader.txt";
std::string fragpath = "/Users/GeorgeQuentin/Dev/OpenGL/OpenGLProject/OpenGL_Essentials/OpenGL_Essentials/resources/fragment-shader.txt";


GLuint shaderProgramID;
GLuint gVAO = 0;
GLuint gVBO = 0;
GLuint texture;
glm::mat4 camera;
glm::mat4 projection;
glm::mat4 view;
glm::mat4 model;
GLfloat gDegreesRotated = 0.0f;
glm::vec3 position;
glm::vec3 intensities; //a.k.a. the color of the light


std::string shaderFromResources(std::string path){
    
    std::ifstream f;
    f.open(path.c_str(), std::ios::in | std::ios::binary);
    if(!f.is_open()){
        throw std::runtime_error(std::string("Failed to open file: ") + path);
    }
    
    //read whole file into stringstream buffer
    std::stringstream buffer;
    buffer << f.rdbuf();
    
    return buffer.str();
}


// loads the file "hazard.png" into gTexture
static void loadTexture(std::string filePath) {
    
  
    // Load texture
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    
//    Bitmap img(filePath.c_str());
//    ////*-----------------------------------------------------------------------------
//    ////*  Make some rgba data (can also load a file here)
//    ////*-----------------------------------------------------------------------------

//    int tw = img.width; 
//    int th = img.height;
    FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
    FIBITMAP* dib(0);
    
    fif = FreeImage_GetFileType(filePath.c_str(), 0); // Check the file signature and deduce its format
    
    if(fif == FIF_UNKNOWN) // If still unknown, try to guess the file format from the file extension
        fif = FreeImage_GetFIFFromFilename(filePath.c_str());
    
    if(fif == FIF_UNKNOWN) // If still unknown, return failure
        return;
    
    if(FreeImage_FIFSupportsReading(fif)) // Check if the plugin has reading capabilities and load the file
        dib = FreeImage_Load(fif, filePath.c_str());
    
    if(!dib) {
        char message[1024];
        sprintf(message, "Cannot load image\n%s\n",filePath.c_str());
        return;
    }
    
    BYTE* pData = FreeImage_GetBits(dib); // Retrieve the image data
    
    // If somehow one of these failed (they shouldn't), return failure
    if (pData == NULL || FreeImage_GetWidth(dib) == 0 || FreeImage_GetHeight(dib) == 0)
        return;
    
    int width = FreeImage_GetWidth(dib); 
    int height = FreeImage_GetHeight(dib);
    int bpp = FreeImage_GetBPP(dib);// bytes per pixel
    
    
    // set wrap mode
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    
    float color[] = { 1.0f, 0.0f, 0.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);
    
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    
    // Nice trilinear filtering.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    
    glGenerateMipmap(GL_TEXTURE_2D);
    
    ///*-----------------------------------------------------------------------------
    ////*  Allocate Memory on the GPU
    //// *-----------------------------------------------------------------------------
    // target | lod | internal_format | width | height | border | format | type | data
    // Give the image to OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pData);
    
//    ////*-----------------------------------------------------------------------------
//    //// *  Load data onto GPU (bitmaps flip RGB order)
//    //// *-----------------------------------------------------------------------------
//    // target | lod | xoffset | yoffset | width | height | format | type | data
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_BGR,GL_UNSIGNED_BYTE, pData);
    
    glGenerateMipmap(GL_TEXTURE_2D);
    
    /////*-----------------------------------------------------------------------------
    //// *  Unbind texture
    ////*-----------------------------------------------------------------------------
    glBindTexture(GL_TEXTURE_2D, 0);

    
}


GLuint loadShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource)
{
    
    //there are shader files that exist on the hard drive
    //you can create, compile, attach, link and use the shader
    
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
    
    //you get the shader program from the drive and attach Link the vertex and fragment shader into a shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    //glBindFragDataLocation(shaderProgram, 0, "out_color");
    
    //then we are going to link all the attached shaders with out game
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
    
    
    // then we can call the user program to use the shader program
    glUseProgram(shaderProgram);
    
    float aspectRatio = (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;
    
    // Projection matrix : 45∞ Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units.
    //set the "projection" uniform in the vertex shader, because it's not going to change
    projection = glm::perspective(glm::radians(50.0f), aspectRatio, 0.1f, 10.0f);
    GLint projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projection" );
    glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(projection));
    
    
    //set the "camera" uniform in the vertex shader, because it's also not going to change
    //passing information to the vertext shader 
    // Camera or view matrix
    camera = glm::lookAt(
                         glm::vec3(3,3,3), // Camera is at (4,3,-3), in World Space
                         glm::vec3(0,0,0), // and looks at the origin
                         glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
                         );
    GLint viewcameraMatrixLocation = glGetUniformLocation(shaderProgram, "camera" );
    glUniformMatrix4fv(viewcameraMatrixLocation, 1, GL_FALSE, glm::value_ptr(camera));
    
    
    //stop using the shader program
    glUseProgram(0);
    
    return shaderProgram;
}

// loads a cube into the VAO and VBO globals: gVAO and gVBO
static void loadCube(const GLuint &shaderProgram) {
    // make and bind the VAO
    glGenVertexArrays(1, &gVAO);
    glBindVertexArray(gVAO);
    
    // make and bind the VBO
    glGenBuffers(1, &gVBO);
    glBindBuffer(GL_ARRAY_BUFFER, gVBO);
    
    // Make a cube out of triangles (two triangles per side)
    GLfloat vertexData[] = {
        //  X     Y     Z       U     V
        // bottom
        -1.0f,-1.0f,-1.0f,   0.0f, 0.0f,
        1.0f,-1.0f,-1.0f,   1.0f, 0.0f,
        -1.0f,-1.0f, 1.0f,   0.0f, 1.0f,
        1.0f,-1.0f,-1.0f,   1.0f, 0.0f,
        1.0f,-1.0f, 1.0f,   1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,   0.0f, 1.0f,
        
        // top
        -1.0f, 1.0f,-1.0f,   0.0f, 0.0f,
        -1.0f, 1.0f, 1.0f,   0.0f, 1.0f,
        1.0f, 1.0f,-1.0f,   1.0f, 0.0f,
        1.0f, 1.0f,-1.0f,   1.0f, 0.0f,
        -1.0f, 1.0f, 1.0f,   0.0f, 1.0f,
        1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
        
        // front
        -1.0f,-1.0f, 1.0f,   1.0f, 0.0f,
        1.0f,-1.0f, 1.0f,   0.0f, 0.0f,
        -1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,   0.0f, 0.0f,
        1.0f, 1.0f, 1.0f,   0.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
        
        // back
        -1.0f,-1.0f,-1.0f,   0.0f, 0.0f,
        -1.0f, 1.0f,-1.0f,   0.0f, 1.0f,
        1.0f,-1.0f,-1.0f,   1.0f, 0.0f,
        1.0f,-1.0f,-1.0f,   1.0f, 0.0f,
        -1.0f, 1.0f,-1.0f,   0.0f, 1.0f,
        1.0f, 1.0f,-1.0f,   1.0f, 1.0f,
        
        // left
        -1.0f,-1.0f, 1.0f,   0.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,   1.0f, 0.0f,
        -1.0f,-1.0f,-1.0f,   0.0f, 0.0f,
        -1.0f,-1.0f, 1.0f,   0.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,   1.0f, 0.0f,
        
        // right
        1.0f,-1.0f, 1.0f,   1.0f, 1.0f,
        1.0f,-1.0f,-1.0f,   1.0f, 0.0f,
        1.0f, 1.0f,-1.0f,   0.0f, 0.0f,
        1.0f,-1.0f, 1.0f,   1.0f, 1.0f,
        1.0f, 1.0f,-1.0f,   0.0f, 0.0f,
        1.0f, 1.0f, 1.0f,   0.0f, 1.0f
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
    
    
    GLsizei stride = 5 * sizeof(GLfloat); //8*sizeof(GLfloat);
    
    // Specify the layout of the vertex data
    // connect the xyz to the "vert" attribute of the vertex shader
    GLint positionAttribute = glGetAttribLocation(shaderProgram, "vert_position");// Vertex positions
    if(positionAttribute == -1){
        throw std::runtime_error(std::string("Program attribute not found: ") + "vert_position");
    }
    glEnableVertexAttribArray(positionAttribute);
   // glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, stride, nullptr);
    glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, stride, nullptr);
    
    // connect the uv coords to the "vertTexCoord" attribute of the vertex shader. 
    GLint colorAttribute = glGetAttribLocation(shaderProgram, "color");
    if(colorAttribute == -1){
        throw std::runtime_error(std::string("Program attribute not found: ") + "color");
    }
    glEnableVertexAttribArray(colorAttribute);
    glVertexAttribPointer(colorAttribute, 2, GL_FLOAT, GL_FALSE,  stride, (const GLvoid*)(3 * sizeof(GLfloat)));
    
    // connect the uv coords to the "vertTexCoord" attribute of the vertex shader. 
//    GLint textureAttribute = glGetAttribLocation(shaderProgram, "vertTexCoord");
//    if(textureAttribute == -1){
//        throw std::runtime_error(std::string("Program attribute not found: ") + "vertTexCoord");
//    }
//    // connect the uv coords to the "vertTexCoord" attribute of the vertex shader
//    glEnableVertexAttribArray(textureAttribute);
//    glVertexAttribPointer(textureAttribute, 2, GL_FLOAT, GL_TRUE,  stride, (const GLvoid*)(3 * sizeof(GLfloat)));
//    
//    
//    // connect the normal to the "vertNormal" attribute of the vertex shader
//    GLint normalAttribute = glGetAttribLocation(shaderProgram, "vertNormal");
//    if(normalAttribute == -1){
//        throw std::runtime_error(std::string("Program attribute not found: ") + "vertNormal");
//    }
//    glEnableVertexAttribArray(normalAttribute);
//    glVertexAttribPointer(normalAttribute, 3, GL_FLOAT, GL_TRUE,  stride, (const GLvoid*)(5 * sizeof(GLfloat)));
//    
//    
    //unbind the VAO
    glBindVertexArray(0);
}


// draws object here
static void Render() {
    
    // Clear the screen
    glClearColor(0.1f, 0.4f, 0.4f, 1.0f); 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // bind the program (the shaders)
    glUseProgram(shaderProgramID);
    
    // Model matrix : an identity matrix (model will be at the origin)
    // set the "model" uniform in the vertex shader, based on the gDegreesRotated global
    model = glm::rotate( glm::mat4(), glm::radians(gDegreesRotated), glm::vec3(0,1,0)  );
    GLint modelMatrixLocation = glGetUniformLocation(shaderProgramID, "model" );
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(model));
    
//    
//    GLint text = glGetUniformLocation(shaderProgramID, "tex" );
//    
//    GLint lightPosition = glGetUniformLocation(shaderProgramID, "light.position" );
//    position = glm::vec3(0.0f, 0.0f, 1.0f);
//    glUniformMatrix3fv(lightPosition, 1, GL_FALSE, glm::value_ptr(position) );
//    
//    GLint lightIntensities = glGetUniformLocation(shaderProgramID, "light.intensities" );
//    intensities = glm::vec3(1,0,0); //red
//    glUniformMatrix3fv(lightIntensities, 1, GL_FALSE, glm::value_ptr(intensities));
//    
//    
    // bind the texture and set the "tex" uniform in the fragment shader
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    // bind the VAO (the triangle)
    glBindVertexArray(gVAO);
    
    // draw the VAO
    glDrawArrays(GL_TRIANGLES, 0, 6*2*3);
    
    // unbind the VAO
    glBindVertexArray(0);
    
    glBindTexture(GL_TEXTURE_2D, 0);
    
    // unbind the program
    glUseProgram(0);
    
}

// update the scene based on the time elapsed since last update
void Update(float secondsElapsed) {
    const GLfloat degreesPerSecond = 180.0f;
    gDegreesRotated += secondsElapsed * degreesPerSecond;
    while(gDegreesRotated > 360.0f) gDegreesRotated -= 360.0f;
}

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

int main(int argc, const char * argv[])  {
    
    // start GL context and O/S window using the GLFW helper library
    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        exit(EXIT_FAILURE);
        return 1;
    } 
    
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//We don't want the old OpenGL and want the core profile
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    
    
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Open GL", nullptr, nullptr);
    if (!window) {
        fprintf(stderr, "ERROR: could not open window with GLFW3\n");
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);
    
    glfwSetErrorCallback(error_callback);
    
    // start GLEW extension handler
    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK)
    {
        glfwTerminate();
        throw std::runtime_error("glewInit failed");
        exit(EXIT_FAILURE);
        return 1;
    }
    
    if(!GLEW_VERSION_3_2){
        throw std::runtime_error("OpenGL 3.2 API is not available.");
    }
    
    // get version info
    const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
    const GLubyte* version = glGetString(GL_VERSION); // version as a string
    const GLubyte* vendor = glGetString(GL_VENDOR); // vendor as a string
    const GLubyte* shaderLanguage = glGetString(GL_SHADING_LANGUAGE_VERSION); // shader lang as a string
    // print out some info about the graphics drivers
    std::cout << "OpenGL version supported: " << version << std::endl;
    std::cout << "GLSL version: " << shaderLanguage << std::endl;
    std::cout << "Renderer: " << renderer << std::endl;
    std::cout << "Vendor: " << vendor << std::endl;
    
    // OpenGL settings
    // tell GL to only draw onto a pixel if the shape is closer to the viewer
    glEnable(GL_DEPTH_TEST); // enable depth-testing
    glEnable(GL_CULL_FACE);//only front facinf poligons are rendered, forn facing have a clock wise or counter clock wise order
    //glDisable(GL_CULL_FACE);
    
    glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    //loading shader
    // load vertex and fragment shaders into opengl
    shaderProgramID = loadShaderProgram(shaderFromResources(vertpath).c_str(), shaderFromResources(fragpath).c_str());
    
    
    
    // create buffer and fill it with the points of the object
    loadCube(shaderProgramID);
    
    //loadTexture(texturepath);
    
    
    // Scale to window size
    GLint windowWidth, windowHeight;
    glfwGetWindowSize(window, &windowWidth, &windowHeight);
    glfwGetFramebufferSize(window, &windowWidth, &windowHeight);
    if (windowHeight == 0) windowHeight = 1; // To prevent divide by 0
    
    // run while the window is open
    double lastTime = glfwGetTime();
    while(!glfwWindowShouldClose(window))
    {
        glViewport(0.0f, 0.0f, windowWidth, windowHeight);
        
        // update the scene based on the time elapsed since last update
        double thisTime = glfwGetTime();
        Update((float)(thisTime - lastTime));
        lastTime = thisTime;
        
        //Render objects
        Render();
        
        // Update Screen, swap the display buffers (displays what was just drawn)
        glfwSwapBuffers(window);
        
        // Check for any input, or window movement
        glfwPollEvents();
        
        // check for errors
        GLenum error = glGetError();
        if(error != GL_NO_ERROR)
            std::cerr << "OpenGL Error " << error << std::endl;
    }
    
    // close GL context and any other GLFW resources
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
    
    return 0;
}


