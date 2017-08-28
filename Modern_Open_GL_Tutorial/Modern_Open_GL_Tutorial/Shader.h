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
    
    void Update(const Transform & transform, 
                Camera * camera, 
                const bool & bUseTexture, 
                const glm::vec3 &lighColor = glm::vec3(0),
                const glm::vec3 &lighPosition = glm::vec3(0) );
        
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
        SAMPLER_U,
        USETEXTURE_U,
        
        LIGHTCOLOR_U,
        LIGHTPOSITION_U,
        LIGHTDIRECTION_U,
        LIGHTWORLDDIRECTION_U,
        LIGHTCUTOFF_U,
        LIGHTOUTERCUTOFF_U,
        LIGHTAMBIENT_U,
        LIGHTDIFFUSE_U,
        LIGHTSPECULAR_U,
        LIGHTCONSTANT_U,
        LIGHTLINEAR_U,
        LIGHTQUADRATIC_U,
        
        VIEWPOSITION_U,
        NORMALSAMPLER_U,
        DIFFUSESAMPLER_U,
        SPECULARSAMPLER_U,
        SHININESS_U,
        INTENSITY_U,
        
        NUM_UNIFORMS
    };
    
    
    static const unsigned int NUM_SHADERS = 2;
    
    GLuint m_program;
    GLuint m_shaders[NUM_SHADERS];
    GLuint m_uniforms[NUM_UNIFORMS];
    
};

#endif /* Shader_h */
