//
//  main.cpp
//  OpenGL_Essentials
//
//  Created by GEORGE QUENTIN on 15/02/2017.
//  Copyright © 2017 LEXI LABS. All rights reserved.
//

#include "Common.h"

#include "Shader.h"
#include "Mesh.h"
#include "Pyramid.h"
#include "Texture.h"

#define SCREEN_WIDTH 1020
#define SCREEN_HEIGHT 720


int main(int argc, const char * argv[])  {
    
    // start GL context and O/S window using the GLFW helper library
    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        exit(EXIT_FAILURE);
        return 1;
    } 
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//We don't want the old OpenGL and want the core profile
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    
    
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Open GL", nullptr, nullptr);
    if (!window) {
        fprintf(stderr, "ERROR: could not open window with GLFW3\n");
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);
    
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
    glEnable(GL_DEPTH_TEST); // enable the use of our depth buffer 
    glEnable(GL_CULL_FACE);//only front facinf poligons are rendered, forn facing have a clock wise or counter clock wise order
    //glDisable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    
    //load shader program
    std::string path = "/Users/GeorgeQuentin/Dev/OpenGL/OpenGLProject/ModernOpenGL_Basic/ModernOpenGL_Basic";
    Shader shader(path + "/res/shaders/basicShader");
    
    
    //load mesh
    Vertex triangleVertices[] = {
        //vertices positions                          //texture             //colors
        Vertex( glm::vec3(0.0f,0.8f,0.0f), glm::vec2(0.0f,0.5f), glm::vec3(0.0f,0.8f,0.0f)   ),
        Vertex( glm::vec3(-0.5f,-0.2f,0.0f), glm::vec2(0.25f,1.0f), glm::vec3(0.8f,0.8f,0.0f) ),
        Vertex( glm::vec3(0.5f,-0.2f,0.0f), glm::vec2(0.5f,0.5f), glm::vec3(0.8f,0.8f,0.0f) )
    };
    
    
    float size = 1.0f;
   
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
    
    //Specify the 8 VERTICES of A Cube
    Vertex cubeUniqueVertices[] = { 
        Vertex( glm::vec3( size, -size, size ), glm::vec2(0.0f,0.0f), glm::vec3( 1.0f, 1.0f, 0.0f ) ), // 0
        Vertex( glm::vec3( size, size, size ), glm::vec2(1.0f,0.0f), glm::vec3( 0.0f, 1.0f, 1.0f ) ), // 1
        Vertex( glm::vec3( -size, size, size ), glm::vec2(0.0f,0.0f), glm::vec3(1.0f, 1.0f, 1.0f ) ), // 2
        Vertex( glm::vec3( -size, -size, size ), glm::vec2(1.0f,0.0f), glm::vec3( 1.0f, 1.0f, 1.0f) ), // 3
        Vertex( glm::vec3( size, -size, -size ), glm::vec2(0.0f,1.0f), glm::vec3( 1.0f, 0.0f, 0.0f ) ), // 4
        Vertex( glm::vec3( size, size, -size ), glm::vec2(1.0f,1.0f), glm::vec3( 0.0f, 0.0f, 1.0f ) ), // 5
        Vertex( glm::vec3( -size, size, -size ), glm::vec2(0.0f,1.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 6
        Vertex( glm::vec3( -size, -size, -size ), glm::vec2(1.0f,1.0f), glm::vec3( 0.0f, 1.0f, 1.0f ) ), // 7
    };
    
    Vertex cubeVertices[] = {
        //vertices positions                          //texture             //colors
        //bottom 3,7,4  4,0,3
        Vertex( glm::vec3(  -size,-size,size ), glm::vec2(1.0f,0.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 0
        Vertex( glm::vec3( -size, -size, -size ), glm::vec2(1.0f,1.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 1
        Vertex( glm::vec3( size, -size, -size ), glm::vec2(0.0f,1.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 2
        Vertex( glm::vec3(  size, -size, -size ), glm::vec2(0.0f,1.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 3
        Vertex( glm::vec3( size, -size, size ), glm::vec2(0.0f,0.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 4
        Vertex( glm::vec3( -size, -size, size ), glm::vec2(1.0f,0.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 5
        
        //top   1,5,6   6,2,1
        Vertex( glm::vec3( size,size, size ), glm::vec2(1.0f,0.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 6
        Vertex( glm::vec3( size, size, -size ), glm::vec2(1.0f,1.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 7
        Vertex( glm::vec3(  -size, size,-size ), glm::vec2(0.0f,1.0f), glm::vec3(1.0f, 1.0f, 1.0f ) ), // 8
        Vertex( glm::vec3( -size, size,-size ), glm::vec2(0.0f,1.0f), glm::vec3( 1.0f, 1.0f, 1.0f) ), // 9
        Vertex( glm::vec3( -size, size, size ), glm::vec2(0.0f,0.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 10
        Vertex( glm::vec3( size, size, size ), glm::vec2(1.0f,0.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 11
        
        //front 0,1,2    2,3,0
        Vertex( glm::vec3(  size,-size,size ), glm::vec2(1.0f,0.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 12
        Vertex( glm::vec3( size, size, size ), glm::vec2(1.0f,1.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 13
        Vertex( glm::vec3( -size, size, size ), glm::vec2(0.0f,1.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 14
        Vertex( glm::vec3( -size,size, size ), glm::vec2(0.0f,1.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 15
        Vertex( glm::vec3(  -size, -size, size ), glm::vec2(0.0f,0.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 16
        Vertex( glm::vec3( size, -size, size ), glm::vec2(1.0f,0.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 17
        
        //back  7,6,5   5,4,7
        Vertex( glm::vec3( -size, -size, -size ), glm::vec2(1.0f,0.0f), glm::vec3(1.0f, 1.0f, 1.0f ) ), // 18
        Vertex( glm::vec3( -size, size, -size ), glm::vec2(1.0f,1.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 19
        Vertex( glm::vec3( size, size, -size ), glm::vec2(0.0f,1.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 20
        Vertex( glm::vec3( size, size, -size ), glm::vec2(0.0f,1.0f), glm::vec3(1.0f, 1.0f, 1.0f ) ), // 21
        Vertex( glm::vec3( size, -size,-size ), glm::vec2(0.0f,0.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 22
        Vertex( glm::vec3( -size, -size, -size ), glm::vec2(1.0f,0.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 23
        
        //left   3,2,6  6,7,3
        Vertex( glm::vec3( -size,-size,size), glm::vec2(1.0f,0.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 24
        Vertex( glm::vec3( -size, size, size ), glm::vec2(1.0f,1.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 25
        Vertex( glm::vec3( -size, size, -size ), glm::vec2(0.0f,1.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 26
        Vertex( glm::vec3( -size, size, -size ), glm::vec2(0.0f,1.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 27
        Vertex( glm::vec3( -size, -size, -size), glm::vec2(0.0f,0.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 28
        Vertex( glm::vec3( -size, -size, size ), glm::vec2(1.0f,0.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 29
        
        //right   4,5,1   1,0,4
        Vertex( glm::vec3( size, -size, -size ), glm::vec2(1.0f,0.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 30
        Vertex( glm::vec3( size, size,-size ), glm::vec2(1.0f,1.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 31
        Vertex( glm::vec3( size, size, size ), glm::vec2(0.0f,1.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 32
        Vertex( glm::vec3( size, size, size ), glm::vec2(0.0f,1.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 33
        Vertex( glm::vec3( size, -size, size ), glm::vec2(0.0f,0.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 34
        Vertex( glm::vec3( size, -size, -size ), glm::vec2(1.0f,0.0f), glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 35
    };
    unsigned int indices[] = { 
        //bottom 
        3,7,4,  
        4,0,3,
        //top   
        1,5,6,
        6,2,1,
        //front 
        0,1,2,
        2,3,0,
        //back  
        7,6,5,
        5,4,7,
        //left   
        3,2,6,
        6,7,3,
        //right   
        4,5,1,
        1,0,4
    }; 
    
    
    
    
    /////---- 4-------
    ////    . . .  .             
    //    .  .   .  .      
    //   .  .     .   .   
    //  .  .       .    .
    // .   2......... |  /3
    //.              . //
    //0--------------/1
    
    Vertex pyramidVertices[] = {
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
    
//    Mesh mesh( pyramidVertices, 
//              sizeof(pyramidVertices)/sizeof(pyramidVertices[0]), 
//              indices,
//              sizeof(indices)/sizeof(indices[0]),
//              false
//              );
    
    bool shouldUseIndices = true;
    Mesh meshWithIndices( cubeUniqueVertices, 
              sizeof(cubeUniqueVertices)/sizeof(cubeUniqueVertices[0]), 
              indices,
              sizeof(indices)/sizeof(indices[0]),
              shouldUseIndices
              );
    
    //load texture;
    Texture texture(path + "/res/textures/spiralcolor.jpg", true);
    
    //setup camera
    float aspect = (float)SCREEN_WIDTH/(float)SCREEN_HEIGHT;
    Camera camera(glm::vec3(0.0f, 0.0f,-15.0f), 70.0f, aspect, 0.1f, 5000.0f);
    
    //transform model
    Transform transform;
    float counter = 0.0f;
    
    
    
    Shader shader2(path + "/res/shaders/basicShader");
    Mesh meshModel(path + "/res/model/monkey3.obj");
    Texture texture2(path + "/res/textures/bricks.jpg", true);
    Transform transform2;
    
    
    while(!glfwWindowShouldClose(window))
    {
        // Clear the screen
        glClearColor(0.1f, 0.4f, 0.4f, 1.0f); 
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//enable and clear the color and depth buffer
        
        //Render process 1
        {
           
            float sinCounter = sinf(counter);
            float conCounter = cosf(counter);
            
            transform.GetPositions()->x = sinCounter;
            transform.GetPositions()->z = conCounter;
            transform.GetRotation()->x = counter * 25;
            transform.GetRotation()->y = counter * 15;
            transform.GetRotation()->z = counter * 5;
            //transform.SetScale(glm::vec3(conCounter, conCounter, conCounter));
            
            // bind the shader program 
            shader.Bind();
            
            
            transform.SetPositions(glm::vec3(-2.0f, 0.0f, 0.0f) );
            
            //update shader, including the tranform of our mesh, and the camera view of the mesh
            shader.Update(transform, camera, false);
            
            //binding texture at zero unit
            texture.Bind(0);
            
            //drawing our meshes
            //mesh.Draw(false);
            
            meshWithIndices.Draw(shouldUseIndices);
            
            
            //unbind texture
            texture.UnBind();
            
            // unbind the shader program
            shader.UnBind();
            
        }
        
        
        //Render process 2
        {
            // bind the shader program 
            shader2.Bind();
            
            transform2.SetPositions(glm::vec3(5.0f, 0.0f, 10.0f) );
            transform2.GetRotation()->y = counter * 25;
            transform2.SetScale(glm::vec3(3,3,3));
            
            //update shader, including the tranform of our mesh, and the camera view of the mesh
            shader2.Update(transform2, camera, true);
            
            //binding texture at zero unit
            texture2.Bind(0);
            
            
            meshModel.Draw(true);
            
            //unbind texture
            texture2.UnBind();
            
            // unbind the shader program
            shader2.UnBind();
            
        }
        
        counter += 0.001f;

        
        // Update Screen, swap the display buffers (displays what was just drawn)
        glfwSwapBuffers(window);
        
        // Check for any input, or window movement
        glfwPollEvents();
        
        // check for errors
        GLenum error = glGetError();
        if(error != GL_NO_ERROR)
            std::cerr << "OpenGL Error: " << error << std::endl;
    }
    
    // close GL context and any other GLFW resources
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
    
    return 0;
}


