#version 410 core

// from https://www.youtube.com/watch?v=vnUQOIODAKc&index=25&list=PLRtjMdoYXLf6zUMDJVRZYV-6g6n62vet8

struct Material
{
    sampler2D sampler;
    sampler2D normalSampler;
    sampler2D diffuseSampler;
    sampler2D specularSampler;
    float shininess; // specularPower
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular; 
};

struct BaseLight
{
    vec3 color;
    float intensity;
};

struct Attenuation
{
    float constant;
    float linear;
    float exponent;
};

struct DirectionalLight
{
    BaseLight base;
    vec3 direction;
};

struct PointLight
{
    BaseLight base;
    Attenuation attenuation;
    vec3 position;
    float range;
};

struct SpotLight
{
    PointLight pointLight;
    vec3 direction;
    float cutoff;
};

uniform Material material;
uniform DirectionalLight directionallight;
uniform PointLight pointlight;
uniform SpotLight spotlight;

uniform vec3 viewPosition;
uniform bool bUseTexture;

in vec3 vEyePosition;
in vec2 vTexCoord;	
in vec3 vEyeNormal;


out vec4 fOutputColor;		// The output colour

void main() {

    fOutputColor = vec4( 1.0f );
}
