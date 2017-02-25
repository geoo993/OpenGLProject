//
//  Shader.cpp
//  ModernOpenGL_Basic
//
//  Created by GEORGE QUENTIN on 25/02/2017.
//  Copyright © 2017 LEXI LABS. All rights reserved.
//

#include "Shader.h"

Shader::Shader(const std::string &fileName){
    //creating a new rogram, create some space in the gpu for our shader program, 
    //open gl will give us some number to refer to it by
    //to think of it, there are actually shader files that exist on the hard drive
    //you can create, compile, attach, link and use the shader, we that during this process
    m_program = glCreateProgram();
    
    //vertex shader
    m_shaders[0] = CreateShader( LoadShader( fileName + ".vert"), GL_VERTEX_SHADER  );
    
    //fragment shader
    m_shaders[1] = CreateShader( LoadShader( fileName + ".frag"), GL_FRAGMENT_SHADER  );
    
    for (unsigned int i = 0; i < NUM_SHADERS; i++){
        glAttachShader(m_program, m_shaders[i]);
    }
    
    //tells open gl what part of data to read in your shader program
    glBindAttribLocation(m_program, 0, "inPosition");
    glBindAttribLocation(m_program, 1, "inTexCoord");
    glBindAttribLocation(m_program, 2, "inNormal");
    
    //one is true and zero is false
    glUniform1i(glGetUniformLocation(m_program, "bUseTexture"), 0);
    
    //glUniform1i(glGetUniformLocation(m_program, "diffuse"), 0);
    
    //we link all the attached shaders with out game
    glLinkProgram(m_program);
    CheckShaderError(m_program, GL_LINK_STATUS, true, "Error: Shader program linking failure. ");
    
    //validation, by checking shader program that it is still valid
    glValidateProgram(m_program);
    CheckShaderError(m_program, GL_VALIDATE_STATUS, true, "Error: Shader program is in-valid. ");
}

Shader::~Shader(){
    
    for (unsigned int i = 0; i < NUM_SHADERS; i++){
        glDetachShader(m_program, m_shaders[i]);
        glDeleteShader(m_shaders[i]);
    }
    
    glDeleteProgram(m_program);
}

GLuint Shader::CreateShader (const std::string &text, GLenum shadertype)
{
    
    // Create and compile the fragment shader
    GLuint shader = glCreateShader(shadertype);
    
    if (shader == 0){
        std::cerr << "Error: Shader creation failed " << std::endl;
        return 0;
    }
    
    //std::cout << text << endl;
    
    //get the shader file string, a list of one c string
    const GLchar * shaderSource = text.c_str();
    //the length of the c string
    GLint shaderSourceLength = text.length();
    
    glShaderSource(shader, 1, &shaderSource, &shaderSourceLength);
    glCompileShader(shader);
    
    CheckShaderError(shader, GL_COMPILE_STATUS, false, "Error: Shader compilation failed! ");
    
    return shader;
}


void Shader::Bind(){
    // bind to use the shader program
    glUseProgram(m_program);
}

void Shader::UnBind(){
    //unbind and stop using the shader program
    glUseProgram(0);
}

std::string Shader::LoadShader(const std::string& fileName)
{
    std::ifstream file;
    file.open((fileName).c_str());
    
    std::string output;
    std::string line;
    
    if(file.is_open())
    {
        while(file.good())
        {
            getline(file, line);
            output.append(line + "\n");
        }
    }
    else
    {
        std::cerr << "Unable to load shader: " << fileName << std::endl;
    }
    
    return output;
}

void Shader::CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage)
{
    GLint success = 0;
    GLchar error[1024] = { 0 };
    
    if(isProgram){
        glGetProgramiv(shader, flag, &success);
    }else{
        glGetShaderiv(shader, flag, &success);
    }
    
    if(success == GL_FALSE)
    {
        if(isProgram)
            glGetProgramInfoLog(shader, sizeof(error), NULL, error);
        else
            glGetShaderInfoLog(shader, sizeof(error), NULL, error);
        
        std::cerr << errorMessage << ": '" << error << "'" << std::endl;
    }
    
}
