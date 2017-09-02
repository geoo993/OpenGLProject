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
#include "Lighting.h"
#include "Camera.h"

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
        VIEWPOSITION_U,
        
        DIRECTIONALLIGHTCOLOR_U,
        DIRECTIONALLIGHTINTENSITY_U,
        DIRECTIONALLIGHTDIRECTION_U,
        
        POINTLIGHTCOLOR_U,
        POINTLIGHTINTENSITY_U,
        POINTLIGHTPOSITION_U,
        POINTLIGHTCONSTANT_U,
        POINTLIGHTLINEAR_U,
        POINTLIGHTEXPONENT_U,
        POINTLIGHTRANGE_U,
        
        SPOTTLIGHTCOLOR_U,
        SPOTLIGHTINTENSITY_U,
        SPOTLIGHTPOSITION_U,
        SPOTLIGHTDIRECTION_U,
        SPOTLIGHTCUTOFF_U,
        SPOTLIGHTCONSTANT_U,
        SPOTLIGHTLINEAR_U,
        SPOTLIGHTEXPONENT_U,
        SPOTLIGHTRANGE_U,
        
        MATERIALNORMALSAMPLER_U,
        MATERIALDIFFUSESAMPLER_U,
        MATERIALSPECULARSAMPLER_U,
        MATERIALSHININESS_U,
        MATERIALINTENSITY_U,
        MATERIALAMBIENT_U,
        MATERIALDIFFUSE_U,
        MATERIALSPECULAR_U,
        
        NUM_UNIFORMS
    };
    
    
    static const unsigned int NUM_SHADERS = 2;
    
    GLuint m_program;
    GLuint m_shaders[NUM_SHADERS];
    GLuint m_uniforms[NUM_UNIFORMS];
    
    std::string m_dirLightName, m_pointLightName, m_spotLightName;
    
    Camera *m_camera;
    glm::vec3 m_mainlightColor;
    glm::vec3 m_mainlightPosition;
    
    void CreateDirectionalLightUniform(const std::string & uniformName);
    void CreatePointLightUniform(const std::string & uniformName);
    void CreateSpotLightUniform(const std::string& uniformName);
    
    void SetDirectionalLightUniform(const std::string& uniformName, const DirectionalLight & directionalLight);
    void SetPointLightUniform(const std::string& uniformName, const PointLight& pointLight);
    void SetSpotLightUniform(const std::string& uniformName, const SpotLight& spotLight);
    
};

#endif /* Shader_h */
