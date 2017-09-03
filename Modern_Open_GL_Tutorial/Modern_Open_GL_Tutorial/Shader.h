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
    
    void Create(const std::string &fileName,
                const GLuint & numberOfDirectionalLights = 0,
                const GLuint & numberOfPointLights = 0,
                const GLuint & numberOfSpottLights = 0);
    void Bind();
    void UnBind();
    
    void SetTransfromUniform(const Transform & transform, 
                             Camera * camera);
    void SetMaterialUniform(const bool & bUseTexture );
        
    GLuint CreateShader (const std::string &text, GLenum shadertype);
    std::string LoadShader(const std::string& fileName);
    void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);
    
    void CreateDirectionalLights(const std::string &directionalLightName, const GLuint & numberOfDirectionalLights );
    void CreatePointLights(const std::string &pointLightName, const GLuint & numberOfPointLights );
    void CreateSpotLights(const std::string &spotLightName, const GLuint & numberOfSpottLights );
        
    void SetDirectionalLightUniform(const DirectionalLight & directionalLight);
    void SetPointLight1Uniform(const PointLight& pointLight, const glm::vec3& position);
    void SetPointLight2Uniform(const PointLight& pointLight, const glm::vec3& position);
    
    void SetSpotLightUniform(const SpotLight& spotLight);
    
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
        
        POINTLIGHTCOLOR_U1,
        POINTLIGHTINTENSITY_U1,
        POINTLIGHTPOSITION_U1,
        POINTLIGHTCONSTANT_U1,
        POINTLIGHTLINEAR_U1,
        POINTLIGHTEXPONENT_U1,
        POINTLIGHTRANGE_U1,
        
        POINTLIGHTCOLOR_U2,
        POINTLIGHTINTENSITY_U2,
        POINTLIGHTPOSITION_U2,
        POINTLIGHTCONSTANT_U2,
        POINTLIGHTLINEAR_U2,
        POINTLIGHTEXPONENT_U2,
        POINTLIGHTRANGE_U2,
        
        SPOTTLIGHTCOLOR_U,
        SPOTLIGHTINTENSITY_U,
        SPOTLIGHTPOSITION_U,
        SPOTLIGHTDIRECTION_U,
        SPOTLIGHTCUTOFF_U,
        SPOTLIGHTOUTERCUTOFF_U,
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
    
    Camera *m_camera;
    glm::vec3 m_mainlightColor;
    glm::vec3 m_mainlightPosition;
    
    void CreateDirectionalLightUniform(const std::string & uniformName);
    void CreatePointLight1Uniform(const std::string & uniformName);
    void CreatePointLight2Uniform(const std::string & uniformName);
    void CreateSpotLightUniform(const std::string& uniformName);
    
    
};

#endif /* Shader_h */
