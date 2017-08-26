//
//  Game.cpp
//  Modern_Open_GL_Tutorial
//
//  Created by GEORGE QUENTIN on 26/08/2017.
//  Copyright © 2017 geomakesgames. All rights reserved.
//

#include "Game.h"

//keyboard and mouse controlls
static int keyPressedCode = -1; 
static int keyReleasedCode = -1; 
static int keyPressedAction = -1; 
static GLfloat mouseX = SCREEN_WIDTH / 2.0f;
static GLfloat mouseY = SCREEN_HEIGHT / 2.0f;

static void OnMouseDown_callback(GLFWwindow* window, int button, int action, int mode){
    //std::cout << "Mouse Down with button: " << button << " and with action: " << action << std::endl;
    //onMouseDown(button,action);
    
    
    
}

static void OnMouseMove_callback(GLFWwindow* window, double xPos, double yPos){
    //std::cout << "Mouse Move x: " << xPos << " and y: " << yPos << std::endl;
    //onMouseDown(button,action);
    
}

static void OnKeyDown_callback( GLFWwindow* window, int key, int scancode, int action, int mode)
{
    keyPressedAction = action;
    
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    
    switch (action) {
        case GLFW_PRESS:
            keyPressedCode = key; 
            break;
        case GLFW_RELEASE:
            keyPressedCode = -1; 
            keyReleasedCode = key;
            break;
        default:
            break;
    }
    
}

// Constructor
Game::Game()
{
    // Init GLFW
    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        exit(EXIT_FAILURE);
        return; 
    }
    
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    
    // Create a GFLWWindow object that we can use for GLFW's functions
    m_window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Open GL", nullptr, nullptr);
    
    if (!m_window) {
        fprintf(stderr, "ERROR: could not open window with GLFW3\n");
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(m_window);
    
    // Set this to true so GLEW knows to use the modern approach to retieving function pointers and extensions
    glewExperimental = GL_TRUE;
    
    // Initialise GLEW to setup the opnegl Function pointers
    if(glewInit() != GLEW_OK){
        glfwTerminate();
        throw std::runtime_error("Failed to initialise GLEW");
        exit(EXIT_FAILURE);
        return;
    }
    
    if(!GLEW_VERSION_3_2){
        throw std::runtime_error("OpenGL 3.2 API is not available.");
        return;
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
    
    //3. Check for specific functionality
    if (GLEW_ARB_vertex_array_object){
        printf("genVertexArrays supported\n");
    }
    if (GLEW_APPLE_vertex_array_object){
        printf("genVertexArrayAPPLE supported\n");
    }
    
    GLint MaxTextureUnits;
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &MaxTextureUnits);
    std::cout << "There are "<< MaxTextureUnits << " texture units supported by GPU. " << std::endl;
    
    // OpenGL settings
    // tell GL to only draw onto a pixel if the shape is closer to the viewer
    glEnable(GL_DEPTH_TEST); // enable the use of our depth buffer
    glEnable(GL_CULL_FACE);//only front facinf poligons are rendered, forn facing have a clock wise or counter clock wise order
    //glDisable(GL_CULL_FACE);
    //glCullFace(GL_BACK);
    //glEnable(GL_BLEND);
    
    //Define the viewport dimensions
    //GLint width, height;
    // returns the framebuffer size, not the window size. 
    //glfwGetFramebufferSize(m_window, &width, &height);
    //glViewport( 0, 0, width, height); // set the viewport
     
}

// Destructor
Game::~Game()
{
    delete m_window;
}

void Game::Initialise(){

    float size = 1.0f;
    vector<Vertex> pyramidVertices = {
        //vertices positions                          //texture             //colors
        //bottom 0,2,3  3,1,0
        Vertex( glm::vec3(  -size,-size,size ), glm::vec2(1.0f,0.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 0
        Vertex( glm::vec3( -size, -size, -size ), glm::vec2(1.0f,1.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 1
        Vertex( glm::vec3( size, -size, -size ), glm::vec2(0.0f,1.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 2
        Vertex( glm::vec3(  size, -size, -size ), glm::vec2(0.0f,1.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 3
        Vertex( glm::vec3( size, -size, size ), glm::vec2(0.0f,0.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 4
        Vertex( glm::vec3( -size, -size, size ), glm::vec2(1.0f,0.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 5
        
        //front   4,0,1
        Vertex( glm::vec3( 0.0f, size, 0.0f ), glm::vec2(1.0f,0.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 6
        Vertex( glm::vec3( -size, -size, size ), glm::vec2(1.0f,1.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 7
        Vertex( glm::vec3(  size, -size,size ), glm::vec2(0.0f,1.0f), glm::vec3(1.0f, 1.0f, 1.0f ) ), // 8
        
        //back    4,3,2
        Vertex( glm::vec3( 0.0f, size, 0.0f ), glm::vec2(0.0f,1.0f), glm::vec3( 1.0f, 1.0f, 1.0f) ), // 9
        Vertex( glm::vec3( size, -size, -size ), glm::vec2(0.0f,0.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 10
        Vertex( glm::vec3( -size, -size, -size ), glm::vec2(1.0f,0.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 11
        
        //left   4,2,0
        Vertex( glm::vec3( 0.0f, size, 0.0f ), glm::vec2(0.0f,1.0f), glm::vec3( 1.0f, 1.0f, 1.0f) ), // 9
        Vertex( glm::vec3( -size, -size, -size ), glm::vec2(0.0f,0.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 10
        Vertex( glm::vec3( -size, -size, size ), glm::vec2(1.0f,0.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 11
        
        //right  4,1,3
        Vertex( glm::vec3( 0.0f, size, 0.0f ), glm::vec2(0.0f,1.0f), glm::vec3( 1.0f, 1.0f, 1.0f) ), // 9
        Vertex( glm::vec3( size, -size, size ), glm::vec2(0.0f,0.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 10
        Vertex( glm::vec3( size, -size, -size ), glm::vec2(1.0f,0.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 11
        
    };
    
    m_pyramidmesh.Create( pyramidVertices.data(),
                         pyramidVertices.size()
                         );
    
    Vertex triangleVertices[] = {
        //vertices positions                          //texture             //colors
        Vertex( glm::vec3(0.0f,0.8f,0.0f), glm::vec2(0.5f,1.0f), glm::vec3(0.0f,1.0f,0.0f)   ),//top
        Vertex( glm::vec3(-0.5f,-0.2f,0.0f), glm::vec2(0.0f,0.0f), glm::vec3(1.0f,0.0f,0.0f) ), //left
        Vertex( glm::vec3(0.5f,-0.2f,0.0f), glm::vec2(1.0f,0.0f), glm::vec3(0.0f,0.0f,1.0f) )//right
    };
    
    Vertex squareVertices[] = {
        //vertices positions                          //texture             //colors
        Vertex( glm::vec3(-0.5f,0.6f,0.0f), glm::vec2(0.0f,1.0f), glm::vec3(0.0f,1.0f,0.0f)   ),//tleft
         Vertex( glm::vec3(-0.5f,-0.6f,0.0f), glm::vec2(0.0f,0.0f), glm::vec3(1.0f,0.0f,0.0f) ), //bleft
        Vertex( glm::vec3(0.5f,-0.6f,0.0f), glm::vec2(1.0f,0.0f), glm::vec3(1.0f,1.0f,0.0f) ),//bright
        Vertex( glm::vec3(-0.5f,0.6f,0.0f), glm::vec2(0.0f,1.0f), glm::vec3(0.0f,1.0f,0.0f)   ),//tleft
        Vertex( glm::vec3(0.5f,-0.6f,0.0f), glm::vec2(1.0f,0.0f), glm::vec3(1.0f,1.0f,0.0f) ),//bright
        Vertex( glm::vec3(0.5f,0.6f,0.0f), glm::vec2(1.0f,1.0f), glm::vec3(0.0f,1.0f,0.0f)   ),//tright
    };
    
    m_trianglemesh.Create( squareVertices,
                          sizeof(squareVertices)/sizeof(squareVertices[0])
                          );
    
    /////6--------------/5
    ////  .           // |
    //2--------------1   |
    //    .          |   |
    //    .          |   |
    //    .          |   |
    //    .          |   |
    //    7......... |   /4
    //               | //
    //3--------------/0
    
    vector<Vertex> cubeVertices = {
        
        // positions          // texture Coords
        //back
        Vertex(glm::vec3(size, -size, -size),glm::vec2(0.0f, 0.0f),glm::vec3(0.0f,  0.0f, -1.0f)),
        Vertex(glm::vec3(-size, -size, -size),glm::vec2(1.0f, 0.0f),glm::vec3(0.0f,  0.0f, -1.0f)),
        Vertex(glm::vec3(-size,  size, -size),glm::vec2(1.0f, 1.0f),glm::vec3(0.0f,  0.0f, -1.0f)),
        Vertex(glm::vec3(-size,  size, -size),glm::vec2(1.0f, 1.0f),glm::vec3(0.0f,  0.0f, -1.0f)),
        Vertex(glm::vec3(size,  size, -size),glm::vec2(0.0f, 1.0f),glm::vec3(0.0f,  0.0f, -1.0f)),
        Vertex(glm::vec3(size, -size, -size),glm::vec2(0.0f, 0.0f),glm::vec3(0.0f,  0.0f, -1.0f)),
        
        //front t1 = 3,0,1  t2=1,2,3
        //front
        Vertex(glm::vec3(-size, -size,  size),glm::vec2(0.0f, 0.0f),glm::vec3(0.0f,  0.0f, 1.0f)),
        Vertex(glm::vec3(size, -size,  size), glm::vec2(1.0f, 0.0f),glm::vec3(0.0f,  0.0f, 1.0f)),
        Vertex(glm::vec3(size,  size,  size), glm::vec2(1.0f, 1.0f),glm::vec3(0.0f,  0.0f, 1.0f)),
        Vertex(glm::vec3(size,  size,  size), glm::vec2(1.0f, 1.0f),glm::vec3(0.0f,  0.0f, 1.0f)),
        Vertex(glm::vec3(-size,  size,  size),glm::vec2(0.0f, 1.0f),glm::vec3(0.0f,  0.0f, 1.0f)),
        Vertex(glm::vec3(-size, -size,  size),glm::vec2(0.0f, 0.0f),glm::vec3(0.0f,  0.0f, 1.0f)),
        
        //right
        Vertex(glm::vec3(-size, -size, -size),glm::vec2(0.0f, 0.0f),glm::vec3(-1.0f,  0.0f,  0.0f)),
        Vertex(glm::vec3(-size, -size,  size), glm::vec2(1.0f, 0.0f),glm::vec3(-1.0f,  0.0f,  0.0f)),
        Vertex(glm::vec3(-size,  size,  size),glm::vec2(1.0f, 1.0f),glm::vec3(-1.0f,  0.0f,  0.0f)),
        Vertex(glm::vec3(-size,  size,  size),glm::vec2(1.0f, 1.0f),glm::vec3(-1.0f,  0.0f,  0.0f)),
        Vertex(glm::vec3(-size,  size, -size),glm::vec2(0.0f, 1.0f),glm::vec3(-1.0f,  0.0f,  0.0f)),
        Vertex(glm::vec3(-size, -size, -size),glm::vec2(0.0f, 0.0f),glm::vec3(-1.0f,  0.0f,  0.0f)),
        
        //left
        Vertex(glm::vec3(size, -size,  size),glm::vec2(0.0f, 0.0f),glm::vec3(1.0f,  0.0f,  0.0f)),
        Vertex(glm::vec3(size, -size, -size), glm::vec2(1.0f, 0.0f),glm::vec3(1.0f,  0.0f,  0.0f)),
        Vertex(glm::vec3(size,  size, -size),glm::vec2(1.0f, 1.0f),glm::vec3(1.0f,  0.0f,  0.0f)),
        Vertex(glm::vec3(size,  size, -size),glm::vec2(1.0f, 1.0f),glm::vec3(1.0f,  0.0f,  0.0f)),
        Vertex(glm::vec3(size,  size,  size),glm::vec2(0.0f, 1.0f),glm::vec3(1.0f,  0.0f,  0.0f)),
        Vertex(glm::vec3(size, -size,  size),glm::vec2(0.0f, 0.0f),glm::vec3(1.0f,  0.0f,  0.0f)),
        
        
        //bottom
        Vertex(glm::vec3(-size, -size, -size),glm::vec2(0.0f, 0.0f),glm::vec3(0.0f, -1.0f,  0.0f)),
        Vertex(glm::vec3(size, -size, -size), glm::vec2(1.0f, 0.0f),glm::vec3(0.0f, -1.0f,  0.0f)),
        Vertex(glm::vec3(size, -size,  size),glm::vec2(1.0f, 1.0f),glm::vec3(0.0f, -1.0f,  0.0f)),
        Vertex(glm::vec3(size, -size,  size),glm::vec2(1.0f, 1.0f),glm::vec3(0.0f, -1.0f,  0.0f)),
        Vertex(glm::vec3(-size, -size,  size),glm::vec2(0.0f, 1.0f),glm::vec3(0.0f, -1.0f,  0.0f)),
        Vertex(glm::vec3(-size, -size, -size),glm::vec2(0.0f, 0.0f),glm::vec3(0.0f, -1.0f,  0.0f)),
        
        //top
        Vertex(glm::vec3(size,  size,  size), glm::vec2(1.0f, 0.0f),glm::vec3(0.0f,  1.0f,  0.0f)),
        Vertex(glm::vec3(size,  size, -size), glm::vec2(1.0f, 1.0f),glm::vec3(0.0f,  1.0f,  0.0f)),
        Vertex(glm::vec3( -size,  size, -size), glm::vec2(0.0f, 1.0f),glm::vec3(0.0f,  1.0f,  0.0f)),
        Vertex(glm::vec3(-size,  size, -size), glm::vec2(0.0f, 1.0f),glm::vec3(0.0f,  1.0f,  0.0f)),
        Vertex(glm::vec3(-size,  size,  size), glm::vec2(0.0f, 0.0f),glm::vec3(0.0f,  1.0f,  0.0f)),
        Vertex(glm::vec3(size,  size,  size), glm::vec2(1.0f, 0.0f),glm::vec3(0.0f,  1.0f,  0.0f))
    };
    
    
    m_cubemesh.Create( cubeVertices.data(), 
                             cubeVertices.size()
                             );
    
    //setup camera
    m_camera.Create(glm::vec3(0.0f, 0.0f,-25.0f), glm::vec3(0.0f, 1.0f,0.0f), PITCH, YAW, ROLL, 70.0f, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT, 0.1f, 5000.0f);
}

void Game::LoadFromResources(const std::string &resourcepath){
    
    //load shader program
    m_basicshader.Create(resourcepath + "/Resources/Shaders/basicShader");
    m_screenshader.Create(resourcepath + "/Resources/Shaders/screenShader");
    m_texture.Create(resourcepath + "/Resources/Textures/bricks.jpg", true);
    //m_texture.Create(resourcepath + "/Resources/Textures/super-mario.jpg", true);
}

void Game::RenderTriangle(){
    
    // bind the shader program
    m_screenshader.Bind();
    
    //update shader, including the tranform of our mesh, and the camera view of the mesh
    m_screenshader.Update(m_trianglemesh.transform, m_camera, true);
    
    //binding texture at zero unit
    m_texture.Bind(0);
    
    m_trianglemesh.Draw();
    
    //unbind texture
    m_texture.UnBind();
    
    // unbind the shader program
    m_screenshader.UnBind();
    
}

void Game::RenderCube(){
    
    glm::vec3 cubesPosition[] = {
        glm::vec3(-11.0f, 5.0f, 22.0f), 
        glm::vec3(-5.0f, 1.0f, -25.0f), 
        glm::vec3(10.0f, 15.0f, 5.0f), 
        glm::vec3(-4.0f, 3.0f, -9.0f), 
        glm::vec3(2.0f, -5.0f, 2.0f), 
        glm::vec3(9.0f, -9.0f, 13.0f), 
        glm::vec3(-8.0f, 12.0f, 8.0f)
    };
    
    // bind the shader program
    m_basicshader.Bind();
    
    m_cubemesh.transform.SetPositions(glm::vec3(-1.0f, 5.0f, 25.0f) );
    m_cubemesh.transform.SetScale(glm::vec3(2,2,2));
    
    //update shader, including the tranform of our mesh, and the camera view of the mesh
    m_basicshader.Update(m_cubemesh.transform, m_camera, true);
    
    //binding texture at zero unit
    m_texture.Bind(0);
    
    m_cubemesh.Draw();
    
    //unbind texture
    m_texture.UnBind();
    
    // unbind the shader program
    m_basicshader.UnBind();
}

void Game::RenderPyramid(){
    
    // bind the shader program
    m_basicshader.Bind();
    
    m_pyramidmesh.transform.SetPositions(glm::vec3(10.0f, 8.0f, 10.0f) );
    m_pyramidmesh.transform.SetScale(glm::vec3(2,2,2));
    
    //update shader, including the tranform of our mesh, and the camera view of the mesh
    m_basicshader.Update(m_pyramidmesh.transform, m_camera, true);
    
    //binding texture at zero unit
    m_texture.Bind(0);
    
    m_pyramidmesh.Draw();
    
    //unbind texture
    m_texture.UnBind();
    
    // unbind the shader program
    m_basicshader.UnBind();
}


void Game::Render(){
    
    RenderPyramid();
    //RenderTriangle();
    RenderCube();
}

void Game::Update(){
    
    Render();
}

void Game::GameLoop(){
    
    Update();
}

void Game::GameTimer(){
    
    
    GLfloat currentframe = glfwGetTime();
    m_deltaTime = currentframe - m_elapsedTime;// detecting time between frame
    
    // Increase the elapsed time and frame counter
    m_elapsedTime += m_deltaTime;
    //    ++m_frameCount;
    //    
    //    m_time += (float) (0.01f * m_deltaTime);
    //    
    //    // Now we want to subtract the current time by the last time that was stored
    //    // to see if the time elapsed has been over a second, which means we found our FPS.
    //    if (m_elapsedTime > 1000)
    //    {
    //        m_elapsedTime = 0;
    //        m_framesPerSecond = m_frameCount;
    //        
    //        // Reset the frames per second
    //        m_frameCount = 0;
    //    }
    //    
}

void Game::DoMovement(){
    
}

void Game::Execute(const std::string &filepath)
{
    
    glfwSetKeyCallback(m_window, OnKeyDown_callback);
    glfwSetCursorPosCallback(m_window, OnMouseMove_callback);
    glfwSetMouseButtonCallback(m_window, OnMouseDown_callback );
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    glfwMakeContextCurrent(m_window);
    
    Initialise();
    
    LoadFromResources(filepath);
    
    while(!glfwWindowShouldClose(m_window))
    {
        
        GameTimer();
        
        // Check for any input, or window movement
        glfwPollEvents();
        
        DoMovement();
        
        // Clear the screen
        glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//enable and clear the color and depth buffer
        
        //Render
        GameLoop();

        // Update Screen, swap the display buffers (displays what was just drawn)
        glfwSwapBuffers(m_window);
        
        
        // check for errors
        GLenum error = glGetError();
        if(error != GL_NO_ERROR)
            std::cerr << "OpenGL Error: " << error << std::endl;
    }
    
    // close GL context and any other GLFW resources
    glfwDestroyWindow(m_window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
    
    return;
}
