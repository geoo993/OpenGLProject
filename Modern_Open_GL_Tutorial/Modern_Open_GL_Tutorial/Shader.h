//
//  Shader.hpp
//
//  Created by GEORGE QUENTIN on 25/02/2017.
//  Copyright © 2017 LEXI LABS. All rights reserved.
//

#ifndef Shader_h
#define Shader_h

#include "Common.h"
#include "Transform.h"

class Shader {
    
public:
    Shader(){}
    ~Shader();
    
    void Create(const std::string &fileName);
    void Bind();
    void UnBind();
    
    void Update(const Transform & transform, const Camera & camera, const bool & bUseTexture);
    
    GLuint CreateShader (const std::string &text, GLenum shadertype);
    std::string LoadShader(const std::string& fileName);
    void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);
    
private:
    Shader(const Shader &other){}
    void operator=(const Shader &other){}
    
    //numbers to refer to our different uniform variables
    enum {
        MVP_U,
        MODEL_U,
        VIEW_U,
        PROJECTION_U,
        USETEXTURE_U,
        //LIGHTDIRECTION_U,
        //DIFFUSECOLOR_U,
        //TWIST_U,
        
        NUM_UNIFORMS
    };
    
    
    static const unsigned int NUM_SHADERS = 2;
    
    GLuint m_program;
    GLuint m_shaders[NUM_SHADERS];
    GLuint m_uniforms[NUM_UNIFORMS];
    
};

#endif /* Shader_h */
