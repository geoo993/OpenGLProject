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


#define SCREEN_WIDTH 720
#define SCREEN_HEIGHT 680


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
    glEnable(GL_DEPTH_TEST); // enable depth-testing
    glEnable(GL_CULL_FACE);//only front facinf poligons are rendered, forn facing have a clock wise or counter clock wise order
    //glDisable(GL_CULL_FACE);
    
    //glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    //load shader program
    std::string path = "/Users/GeorgeQuentin/Dev/OpenGL/OpenGLProject/ModernOpenGL_Basic/ModernOpenGL_Basic";
    Shader shader(path + "/res/shaders/basicShader");
    
    
    //load mesh
    Vertex vertices[] = {
        Vertex( glm::vec3(0.0f,0.8f,0.0f), glm::vec2(0.0f,0.5f), glm::vec3(0.0f,0.2f,0.0f)   ),
        Vertex( glm::vec3(-0.5f,-0.2f,0.0f), glm::vec2(0.25f,1.0f), glm::vec3(0.2f,0.1f,0.0f) ),
        Vertex( glm::vec3(0.5f,-0.2f,0.0f), glm::vec2(0.5f,0.5f), glm::vec3(0.5f,0.6f,0.0f) )
    };
    
    Mesh mesh(vertices, sizeof(vertices)/sizeof(vertices[0]) );
    
    
    //load texture;
    Texture texture(path + "/res/textures/spiralcolor.jpg", true);
    
    
    while(!glfwWindowShouldClose(window))
    {
        // Clear the screen
        glClearColor(0.1f, 0.4f, 0.4f, 1.0f); 
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        //Render process
        {
            
            // bind the shader program 
            shader.Bind();
            
            texture.Bind(0);//binding to zero
            
            mesh.Draw();
            
            //unbind texture
            texture.UnBind();
            
            // unbind the shader program
            shader.UnBind();
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


