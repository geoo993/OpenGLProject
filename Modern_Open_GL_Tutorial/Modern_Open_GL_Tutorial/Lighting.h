//
//  Lighting.hpp
//  Modern_Open_GL_Tutorial
//
//  Created by GEORGE QUENTIN on 02/09/2017.
//  Copyright © 2017 geomakesgames. All rights reserved.
//

#ifndef Lighting_h
#define Lighting_h

#include "Common.h"

class BaseLight 
{
    
public:
    glm::vec3 color;
    float intensity;
    
    BaseLight(const glm::vec3 & color = glm::vec3(0.0f,0.0f,0.0f), float intensity = 0.0f) :
    color(color),
    intensity(intensity) {}
    
    virtual ~BaseLight();
    
private:
    BaseLight(BaseLight& other) {}
    void operator=(BaseLight& other) {}
    
};

struct DirectionalLight : public BaseLight
{
    DirectionalLight(const glm::vec3 & color = glm::vec3(0.0f,0.0f,0.0f), float intensity = 0.0f);
};

struct Attenuation
{
    float constant;
    float linear;
    float exponent;
    
    Attenuation(float constant = 0.0f, float linear = 0.0f, float exponent = 1.0f) :
    constant(constant),
    linear(linear),
    exponent(exponent) {}
};

struct PointLight : public BaseLight
{
    Attenuation atten;
    float range;
    
    PointLight(const glm::vec3 & color = glm::vec3(0.0f,0.0f,0.0f), float intensity = 0.0f, const Attenuation& atten = Attenuation());
};

struct SpotLight : public PointLight
{
    float cutoff;         // between 0 - 1
    float outerCutoff;    // between 0 - 1
    
    SpotLight(const glm::vec3 & color = glm::vec3(0.0f,0.0f,0.0f), float intensity = 0.0f, const Attenuation& atten = Attenuation(), float cutoff = 0.0f, float outerCutoff = 0.0f);
};

#endif /* Lighting_h */
