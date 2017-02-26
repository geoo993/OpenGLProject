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
    //glEnable(GL_DEPTH_TEST); // enable depth-testing
    //glEnable(GL_CULL_FACE);//only front facinf poligons are rendered, forn facing have a clock wise or counter clock wise order
    //glDisable(GL_CULL_FACE);
    
    //load shader program
    std::string path = "/Users/GeorgeQuentin/Dev/OpenGL/OpenGLProject/ModernOpenGL_Basic/ModernOpenGL_Basic";
    Shader shader(path + "/res/shaders/basicShader");
    
    
    //load mesh
    Vertex triangleVertices[] = {
        Vertex( glm::vec3(0.0f,0.8f,0.0f), glm::vec2(0.0f,0.5f), glm::vec3(0.0f,0.8f,0.0f)   ),
        Vertex( glm::vec3(-0.5f,-0.2f,0.0f), glm::vec2(0.25f,1.0f), glm::vec3(0.8f,0.8f,0.0f) ),
        Vertex( glm::vec3(0.5f,-0.2f,0.0f), glm::vec2(0.5f,0.5f), glm::vec3(0.8f,0.8f,0.0f) )
    };
    
    float size = 1.0f;
    
    // Define the 6 vertices of a unit tetrahedron
    Vertex tetrahedronVertices[] = {
        //vertices positions                          //colors
        Vertex( glm::vec3(  0.0f,  size,  0.0f ), glm::vec2(1.0f,1.0f),glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 0
        Vertex( glm::vec3( -size,  -(size/2.0f),  size ), glm::vec2(1.0f,1.0f), glm::vec3( 0.0f, 1.0f, 1.0f ) ), // 1
        Vertex( glm::vec3( 1.0f, -(size/2.0f),  size ), glm::vec2(1.0f,1.0f), glm::vec3( 0.0f, 0.0f, 1.0f ) ), // 2
        Vertex( glm::vec3(  0.0f, -(size/2.0f),  -size ), glm::vec2(1.0f,1.0f), glm::vec3( 1.0f, 0.0f, 1.0f ) ), // 3
        Vertex( glm::vec3(  0.0f, size, 0.0f ), glm::vec2(1.0f,1.0f), glm::vec3( 1.0f, 0.0f, 0.0f ) ), // 4
        Vertex( glm::vec3( -size, -(size/2.0f), size ), glm::vec2(1.0f,1.0f), glm::vec3( 0.0f, 0.0f, 0.0f ) ) // 5
    };
    
    
    // Define the 8 vertices of a unit cube
    Vertex cubeVertices[] = {
        //vertices positions                          //colors
        Vertex( glm::vec3(  size,  size,  size ), glm::vec2(1.0f,1.0f),glm::vec3( 1.0f, 1.0f, 1.0f ) ), // 0
        Vertex( glm::vec3( -size,  size,  size ), glm::vec2(1.0f,1.0f), glm::vec3( 0.0f, 1.0f, 1.0f ) ), // 1
        Vertex( glm::vec3( -size, -size,  size ), glm::vec2(1.0f,1.0f), glm::vec3( 0.0f, 0.0f, 1.0f ) ), // 2
        Vertex( glm::vec3(  size, -size,  size ), glm::vec2(1.0f,1.0f), glm::vec3( 1.0f, 0.0f, 1.0f ) ), // 3
        Vertex( glm::vec3(  size, -size, -size ), glm::vec2(1.0f,1.0f), glm::vec3( 1.0f, 0.0f, 0.0f ) ), // 4
        Vertex( glm::vec3( -size, -size, -size ), glm::vec2(1.0f,1.0f), glm::vec3( 0.0f, 0.0f, 0.0f ) ), // 5
        Vertex( glm::vec3( -size,  size, -size ), glm::vec2(1.0f,1.0f), glm::vec3( 0.0f, 1.0f, 0.0f ) ), // 6
        Vertex( glm::vec3(  size,  size, -size ), glm::vec2(1.0f,1.0f), glm::vec3( 1.0f, 1.0f, 0.0f ) ), // 7
    };
    
    Mesh mesh(triangleVertices, sizeof(triangleVertices)/sizeof(triangleVertices[0]) );
    
    
    //load texture;
    Texture texture(path + "/res/textures/spiralcolor.jpg", true);
    
    //setup camera
    float aspect = (float)SCREEN_WIDTH/(float)SCREEN_HEIGHT;
    Camera camera(glm::vec3(0,0,-3), 70.0f, aspect, 0.1f, 5000.0f);
    
    //transform model
    Transform transform;
    float counter = 0.0f;
    
    
    while(!glfwWindowShouldClose(window))
    {
        // Clear the screen
        glClearColor(0.1f, 0.4f, 0.4f, 1.0f); 
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        //Render process
        {
            float sinCounter = sinf(counter);
            float conCounter = cosf(counter);
            
            transform.GetPositions().x = sinCounter;
            transform.GetPositions().z = conCounter;
            transform.GetRotation().x = counter * 40;
            transform.GetRotation().y = counter * 40;
            transform.GetRotation().z = counter * 40;
            //transform.SetScale(glm::vec3(conCounter, conCounter, conCounter));
            
            // bind the shader program 
            shader.Bind();
            
            //update shader, including the tranform of our mesh, and the camera view of the mesh
            shader.Update(transform, camera);
            
            
            //binding texture at zero unit
            texture.Bind(0);
            
            
            //drawing our mesh
            mesh.Draw();
            
            //unbind texture
            texture.UnBind();
            
            // unbind the shader program
            shader.UnBind();
            
            
            counter += 0.001f;
        }
        
        
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


