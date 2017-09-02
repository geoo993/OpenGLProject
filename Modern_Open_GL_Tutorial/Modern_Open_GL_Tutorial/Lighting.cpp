//
//  Lighting.cpp
//  Modern_Open_GL_Tutorial
//
//  Created by GEORGE QUENTIN on 02/09/2017.
//  Copyright © 2017 geomakesgames. All rights reserved.
//

#include "Lighting.h"

#define COLOR_DEPTH 256

BaseLight::~BaseLight()
{
    
}

DirectionalLight::DirectionalLight(const glm::vec3 & color, float intensity) :
BaseLight(color, intensity)
{
    //SetShader(new Shader("forward-directional"));
}

PointLight::PointLight(const glm::vec3 & color, float intensity, const Attenuation& atten) :
BaseLight(color, intensity),
atten(atten)
{
    vector<float> colorVec = {
        color.x,
        color.y,
        color.z
    };
    float colorMax = *max_element(colorVec.begin(), colorVec.end());
    
    float a = atten.exponent;
    float b = atten.linear;
    float c = atten.constant - COLOR_DEPTH * intensity * colorMax;
    
    this->range = (-b + sqrtf( b * b - 4.0f * a * c)) / ( 2.0f * a);
    
    //SetShader(new Shader("forward-point"));
}

SpotLight::SpotLight(const glm::vec3 & color, float intensity, const Attenuation& atten, float cutoff, float outerCutoff) :
PointLight(color, intensity, atten),
cutoff(cutoff),
outerCutoff(outerCutoff)
{
    //SetShader(new Shader("forward-spot"));
}
