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
        
        DIRECTIONALLIGHTWORLDDIRECTION_U,
        DIRECTIONALLIGHTAMBIENT_U,
        DIRECTIONALLIGHTDIFFUSE_U,
        DIRECTIONALLIGHTSPECULAR_U,
        
        LIGHTCOLOR_U,
        LIGHTINTENSITY_U,
        
        POINTLIGHTPOSITION_A_U,
        POINTLIGHTPOSITION_B_U,
        POINTLIGHTPOSITION_C_U,
        POINTLIGHTPOSITION_D_U,
        
        POINTLIGHTAMBIENT_U,
        POINTLIGHTDIFFUSE_U,
        POINTLIGHTSPECULAR_U,
        POINTLIGHTCONSTANT_U,
        POINTLIGHTLINEAR_U,
        POINTLIGHTQUADRATIC_U,
        
        SPOTLIGHTPOSITION_U,
        SPOTLIGHTDIRECTION_U,
        SPOTLIGHTCUTOFF_U,
        SPOTLIGHTOUTERCUTOFF_U,
        SPOTLIGHTAMBIENT_U,
        SPOTLIGHTDIFFUSE_U,
        SPOTLIGHTSPECULAR_U,
        SPOTLIGHTCONSTANT_U,
        SPOTLIGHTLINEAR_U,
        SPOTLIGHTQUADRATIC_U,
        
        VIEWPOSITION_U,
        
        MATERIALNORMALSAMPLER_U,
        MATERIALDIFFUSESAMPLER_U,
        MATERIALSPECULARSAMPLER_U,
        MATERIALSHININESS_U,
        MATERIALAMBIENT_U,
        MATERIALDIFFUSE_U,
        MATERIALSPECULAR_U,
        
        NUM_UNIFORMS
    };
    
    
    static const unsigned int NUM_SHADERS = 2;
    
    GLuint m_program;
    GLuint m_shaders[NUM_SHADERS];
    GLuint m_uniforms[NUM_UNIFORMS];
    
};

#endif /* Shader_h */
