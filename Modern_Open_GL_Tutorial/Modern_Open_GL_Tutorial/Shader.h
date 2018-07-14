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
    
    void SetTransfromUniform(const Transform & transform, 
                             Camera * camera);
    void SetCameraUniform(const std::string &uniformName, Camera * camera);
    void SetMaterialUniform(const std::string &uniformName,
                            const glm::vec3 &color = glm::vec3(1.0f),
                            const GLfloat & shininess = 32.0f);
    void SetDeclaredUniform(const bool & bUseTexture,
                            const bool & bUseDirectionalLight = true, 
                            const bool & bUsePointLight = true,
                            const bool & bUseSpotlight = true);
        
    GLuint CreateShader (const std::string &text, GLenum shadertype);
    std::string LoadShader(const std::string& fileName);
    void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);

    void SetBaseLightUniform(const std::string &uniformName,
                                    const BaseLight & baseLight);

    void SetDirectionalLightUniform(
                                    const std::string &uniformName, 
                                    const DirectionalLight & directionalLight, 
                                    const glm::vec3& direction);
    void SetPointLightUniform(
                              const std::string &uniformName, 
                              const PointLight& pointLight);
    
    void SetSpotLightUniform(const std::string &uniformName, const SpotLight& spotLight, Camera *camera);
    
    // Setting vectors
    void SetUniform(std::string sName, glm::vec2* vVectors, int iCount = 1);
    void SetUniform(std::string sName, const glm::vec2 vVector);
    void SetUniform(std::string sName, glm::vec3* vVectors, int iCount = 1);
    void SetUniform(std::string sName, const glm::vec3 vVector);
    void SetUniform(std::string sName, glm::vec4* vVectors, int iCount = 1);
    void SetUniform(std::string sName, const glm::vec4 vVector);
    
    // Setting floats
    void SetUniform(std::string sName, float* fValues, int iCount = 1);
    void SetUniform(std::string sName, const float fValue);
    
    // Setting 3x3 matrices
    void SetUniform(std::string sName, glm::mat3* mMatrices, int iCount = 1);
    void SetUniform(std::string sName, const glm::mat3 mMatrix);
    
    // Setting 4x4 matrices
    void SetUniform(std::string sName, glm::mat4* mMatrices, int iCount = 1);
    void SetUniform(std::string sName, const glm::mat4 mMatrix);
    
    // Setting integers
    void SetUniform(std::string sName, int* iValues, int iCount = 1);
    void SetUniform(std::string sName, const int iValue);
    
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

        CAMERAPOSITION_U,
        CAMERAFRONT_U,

        MATERIALNORMALSAMPLER_U,
        MATERIALDIFFUSESAMPLER_U,
        MATERIALSPECULARSAMPLER_U,
        MATERIALSHININESS_U,
        MATERIALCOLOR_U,
//        MATERIALAMBIENT_U,
//        MATERIALDIFFUSE_U,
//        MATERIALSPECULAR_U,
        
        NUM_UNIFORMS
    };
    
    
    static const unsigned int NUM_SHADERS = 2;
    
    GLuint m_program;
    GLuint m_shaders[NUM_SHADERS];
    GLuint m_uniforms[NUM_UNIFORMS];

    void CreateTransformUniform();
    void CreateDeclaredUniform();
    void CreateCameraUniform(const std::string & uniformName);
    void CreateMaterialUniform(const std::string & uniformName);
    
};

#endif /* Shader_h */
