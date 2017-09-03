//
//  Game.cpp
//  Modern_Open_GL_Tutorial
//
//  Created by GEORGE QUENTIN on 26/08/2017.
//  Copyright © 2017 geomakesgames. All rights reserved.
//

#include "Game.h"

//keyboard and mouse controlls
static bool keys[1024];
static int keyPressedCode,keyReleasedCode,keyPressedAction = -1; 
bool firstMouse = true;
GLfloat lastX = SCREEN_WIDTH / 2.0f;
GLfloat lastY = SCREEN_HEIGHT / 2.0f;
static GLfloat mouseX, mouseY = 0.0f;
static GLuint mouseHit, mouseAction = 0.0f;

static void OnMouseDown_callback(GLFWwindow* window, int button, int action, int mode){
    //std::cout << "Mouse Down with button: " << button << " and with action: " << action << std::endl;
    mouseHit = button;
    mouseAction = action;
}


static void OnMouseMove_callback(GLFWwindow* window, double xPosition, double yPosition){
    //std::cout << "Mouse Move x: " << mouseX << " and y: " << mouseY << std::endl;
    mouseX = (GLfloat)xPosition;
    mouseY = (GLfloat)yPosition;
}	

void OnKeyDown_callback( GLFWwindow* window, int key, int scancode, int action, int mode)
{
    keyPressedAction = action;
    
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    if ( key >= 0 && key < 1024 )
    {
        if( action == GLFW_PRESS )
        {
            keys[key] = true;
            keyPressedCode = key; 
        }
        else if( action == GLFW_RELEASE )
        {
            keys[key] = false;
            keyPressedCode = -1; 
            keyReleasedCode = key;
        }
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
    delete m_camera;
}

void Game::Initialise(){

    float size = 1.0f;
    
    //////////////////////////////////////////////////////////////////////////
    ///////////////////////////PYRAMID MODEL/////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    /////---- 4-------
    ////    . . .  .
    //    .  .   .  .
    //   .  .     .   .
    //  .  .       .    .
    // .   2......... |  /3
    //.              . //
    //0--------------/1
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
        Vertex( glm::vec3( 0.0f, size, 0.0f ), glm::vec2(0.0f,1.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 6
        Vertex( glm::vec3( -size, -size, size ), glm::vec2(0.0f,0.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 7
        Vertex( glm::vec3(  size, -size,size ), glm::vec2(1.0f,0.0f), glm::vec3(1.0f, 1.0f, 1.0f ) ), // 8
        
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
    m_lampmesh.Create( cubeVertices.data(), 
                      cubeVertices.size()
                      );
    
    //setup camera
    m_camera = new Camera();
    m_camera->Create(glm::vec3(0.0f, 0.0f,10.0f), glm::vec3(0.0f, 1.0f,0.0f), PITCH, YAW, ROLL, 45.0f, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT, 0.1f, 1000.0f);
    
    // setup light
    m_lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
    m_viewPosition = glm::vec3(10.0f, 10.0f, 22.0f);
    
}

void Game::LoadFromResources(const std::string &resourcepath){
    
    //load shader program
    m_basicshader.Create(resourcepath + "/Resources/Shaders/basicShader");
    m_screenshader.Create(resourcepath + "/Resources/Shaders/screenShader");
    m_lightingshader.Create(resourcepath + "/Resources/Shaders/lightingShader");
    m_lampshader.Create(resourcepath + "/Resources/Shaders/lampShader");
    m_lightshader.Create(resourcepath + "/Resources/Shaders/lightShader", 1, m_pointLightPositions.size(), 1);
}


void Game::RenderPyramid(){
    
    // bind the shader program
    m_basicshader.Bind();
    
    m_pyramidmesh.transform.SetPositions(glm::vec3(10.0f, 8.0f, 10.0f) );
    m_pyramidmesh.transform.SetScale(glm::vec3(3.0f));
    
    //update shader, including the tranform of our mesh, and the camera view of the mesh
    m_basicshader.SetTransfromUniform(m_pyramidmesh.transform, m_camera);
    m_basicshader.SetMaterialUniform( true);
    
    m_pyramidmesh.Draw(0);
    
    // unbind the shader program
    m_basicshader.UnBind();
}

void Game::RenderLamp(){
    
    
    //m_lightColor.r = sin( glfwGetTime() * 2.0f );
    //m_lightColor.g = sin( glfwGetTime() * 0.7f );
    //m_lightColor.b = sin( glfwGetTime() * 1.3f );
    
    for ( GLuint i = 0; i < m_pointLightPositions.size(); ++i){
        
        // bind the shader program
        m_lampshader.Bind();
        
        m_lampmesh.transform.SetPositions(m_pointLightPositions[i]);//m_lightPosition );
        m_lampmesh.transform.SetScale(glm::vec3(0.4f));
        
        //update shader, including the tranform of our mesh, and the camera view of the mesh
        m_lampshader.SetTransfromUniform(m_lampmesh.transform, m_camera);
        m_lampshader.SetMaterialUniform( false);
        
        m_lampmesh.Draw(0);
        
        // unbind the shader program
        m_lampshader.UnBind();
        
    }
    
}

void Game::RenderCubes(){
    
    // bind the shader program
    m_lightshader.Bind();
    
    for ( GLuint i = 0; i < m_cubesPosition.size(); ++i){
        
        GLfloat angle = 20.0f * (GLfloat)i;
        
        m_cubemesh.transform.SetPositions(m_cubesPosition[i] );
        m_cubemesh.transform.SetRotation(glm::vec3(1.0f, angle, 1.0f));
        m_cubemesh.transform.SetScale(glm::vec3(2.0f));
        
        //update shader, including the tranform of our mesh, and the camera view of the mesh
        m_lightshader.SetTransfromUniform(m_cubemesh.transform, m_camera);
        m_lightshader.SetMaterialUniform( false);
        
        m_cubemesh.Draw(0);
        
    }
    
    for ( GLuint i = 0; i < 1; ++i){
        // Directional light
        DirectionalLight dirLight(m_lightColor, 0.2f);
        m_lightshader.SetDirectionalLightUniform(dirLight);
    }
    
    //for ( GLuint i = 0; i < m_pointLightPositions.size(); ++i){
        //m_lightColor
        PointLight pointLight1(m_lightColor, 3.7f, Attenuation(1.0f, 0.09f, 0.32f));
        m_lightshader.SetPointLight1Uniform(pointLight1, m_pointLightPositions[0]);
    //}
    
    PointLight pointLight2(m_lightColor, 3.7f, Attenuation(1.0f, 0.09f, 0.32f));
    m_lightshader.SetPointLight2Uniform(pointLight2, m_pointLightPositions[1]);
    
    SpotLight spotLight(m_lightColor, 24.5f, Attenuation(1.0f, 0.09f, 0.32f), 0.6f, 0.85f);
    m_lightshader.SetSpotLightUniform( spotLight);
    
    // unbind the shader program
    m_lightshader.UnBind();
    
}

void Game::RenderCube(){
    
    // bind the shader program
    m_lightshader.Bind();
    
    glm::vec3 pos(-4.0f, 2.0f, 1.0f);
    
    m_cubemesh.transform.SetPositions(pos );
    //m_cubemesh.transform.SetScale(glm::vec3(2.0f));
    
    //update shader, including the tranform of our mesh, and the camera view of the mesh
    m_lightshader.SetTransfromUniform(m_cubemesh.transform, m_camera);
    m_lightshader.SetMaterialUniform( false);
    
    m_cubemesh.Draw(0);
    
    // unbind the shader program
    m_lightshader.UnBind();
    
    
}
void Game::Render(){
    
    //RenderPyramid();
    //RenderTriangle();
    RenderCubes();
    RenderLamp();
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
    m_elapsedTime = currentframe;
    
}

void Game::DoKeysMovement(bool *selectedkeys){
 
    // Camera controls
    if( selectedkeys[GLFW_KEY_UP] )
    {
        m_camera->ProcessKeyboard( FORWARD, m_deltaTime );
    }
    
    if( selectedkeys[GLFW_KEY_DOWN] )
    {
        m_camera->ProcessKeyboard( BACKWARD, m_deltaTime );
    }
    
    if( selectedkeys[GLFW_KEY_LEFT] )
    {
        m_camera->ProcessKeyboard( LEFT, m_deltaTime );
    }
    
    if( selectedkeys[GLFW_KEY_RIGHT] )
    {
        m_camera->ProcessKeyboard( RIGHT, m_deltaTime );
    }
    
    if( selectedkeys[GLFW_KEY_1])
    {
        m_pointLightPositionsIndex += 1;
        if (m_pointLightPositionsIndex > m_pointLightPositions.size() - 1) {
            m_pointLightPositionsIndex = 0;
        }
    }
   
    if( selectedkeys[GLFW_KEY_W])
    {
        glm::vec3 pos(
                 m_pointLightPositions[m_pointLightPositionsIndex].x, 
                 m_pointLightPositions[m_pointLightPositionsIndex].y + 0.1f, 
                 m_pointLightPositions[m_pointLightPositionsIndex].z);
        m_pointLightPositions[m_pointLightPositionsIndex] = pos;
    }
    
    if( selectedkeys[GLFW_KEY_S] )
    {
        glm::vec3 pos(
                      m_pointLightPositions[m_pointLightPositionsIndex].x,
                      m_pointLightPositions[m_pointLightPositionsIndex].y - 0.1f,
                      m_pointLightPositions[m_pointLightPositionsIndex].z);
        m_pointLightPositions[m_pointLightPositionsIndex] = pos;
        
    }
    
    if( selectedkeys[GLFW_KEY_A] )
    {
        glm::vec3 pos(
                      m_pointLightPositions[m_pointLightPositionsIndex].x - 0.1f,
                      m_pointLightPositions[m_pointLightPositionsIndex].y,
                      m_pointLightPositions[m_pointLightPositionsIndex].z);
        m_pointLightPositions[m_pointLightPositionsIndex] = pos;
        
    }
    
    if( selectedkeys[GLFW_KEY_D] )
    {
        glm::vec3 pos(
                      m_pointLightPositions[m_pointLightPositionsIndex].x + 0.1f,
                      m_pointLightPositions[m_pointLightPositionsIndex].y,
                      m_pointLightPositions[m_pointLightPositionsIndex].z);
        m_pointLightPositions[m_pointLightPositionsIndex] = pos;
        
    }
    
    if( selectedkeys[GLFW_KEY_Z] )
    {
        glm::vec3 pos(
                      m_pointLightPositions[m_pointLightPositionsIndex].x,
                      m_pointLightPositions[m_pointLightPositionsIndex].y,
                      m_pointLightPositions[m_pointLightPositionsIndex].z + 0.1f);
        m_pointLightPositions[m_pointLightPositionsIndex] = pos;
    }
    
    if( selectedkeys[GLFW_KEY_X] )
    {
        glm::vec3 pos(
                      m_pointLightPositions[m_pointLightPositionsIndex].x,
                      m_pointLightPositions[m_pointLightPositionsIndex].y,
                      m_pointLightPositions[m_pointLightPositionsIndex].z - 0.1f);
        m_pointLightPositions[m_pointLightPositionsIndex] = pos;
    }
    
}

void Game::DoMouseMovement(const GLfloat &mouseX, const GLfloat &mouseY){
    
    if( firstMouse )
    {
        lastX = mouseX;
        lastY = mouseY;
        firstMouse = false;
    }
    
    GLfloat xOffset = mouseX - lastX;
    GLfloat yOffset = lastY - mouseY;  // Reversed since y-coordinates go from bottom to left
    
    lastX = mouseX;
    lastY = mouseY;
    
    m_camera->ProcessMouseMovement( xOffset, yOffset );
    
}

void Game::DoMouseHit(const GLuint &hit, const GLuint &action){
    
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
        
        DoKeysMovement(keys);
        DoMouseMovement(mouseX, mouseY);
        DoMouseHit(mouseHit, mouseAction);
        
        // Clear the screen
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
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
