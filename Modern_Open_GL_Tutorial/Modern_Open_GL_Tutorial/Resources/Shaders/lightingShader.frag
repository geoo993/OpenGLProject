#version 410 core

#define NUMBER_OF_POINT_LIGHTS 4

struct Material
{
    sampler2D sampler;
    sampler2D normalSampler;
    sampler2D diffuseSampler;
    sampler2D specularSampler;
    float shininess;
    float intensity;
};


//struct Light
//{
//    vec3 color;
//    vec3 position;
//    vec3 direction;
//    vec3 worldDirection;
//    
//    float cutOff;
//    float outerCutOff;
//    
//    vec3 ambient;
//    vec3 diffuse;
//    vec3 specular;
//    
//    float constant;
//    float linear;
//    float quadratic;
//};


struct DirectionalLight
{
    vec3 color;
    vec3 worldDirection;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
};

struct PointLight
{
    vec3 color;
    vec3 position;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
    float constant;
    float linear;
    float quadratic;
};

struct SpotLight
{
    vec3 color;
    vec3 position;
    vec3 direction;
    vec3 worldDirection;
    
    float cutOff;
    float outerCutOff;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
    float constant;
    float linear;
    float quadratic;
};

uniform Material material;
uniform Light light;
uniform vec3 viewPosition;
uniform vec3 spotLightPosition;
uniform bool bUseTexture;

in vec3 worldPosition;
in vec2 texCoord;
in vec3 localNormal;

out vec4 fOutputColor;		// The output colour

bool bUseDark = false;

bool bUseDirectional = false;  ///directional light else basic lamp illumination

bool bUsePointLight = true; //point light, uses Attenuation and illuminates objects closer to lamp

bool bUseSpotLight = true; //spot light, emitting light from camera


void main() {
    
    if (bUseDirectional) {
        bUseDark = true;
    }
    
    vec3 diffuseColor = light.ambient * light.color ;
    vec3 ambientColor = bUseDark ? light.diffuse * diffuseColor : light.diffuse ;
    vec3 specularColor = light.specular * light.color;
    
    vec3 matDiffuse = vec3(1.0f, 0.5f, 0.31f);
    vec3 matAmbient = vec3(1.0f, 0.5f, 0.31f);
    vec3 matSpecular = vec3(0.5f, 0.5f, 0.5f);
    
    // Ambient
    vec3 ambient = bUseTexture ? 
    ambientColor * vec3(texture(material.diffuseSampler, texCoord)) :
    ambientColor * matAmbient;
    
    vec3 lightPosition = light.position;
    
    // light position is equal to camera view position
    if (bUseSpotLight) {
        lightPosition = viewPosition;
        bUsePointLight = true;
    }
    if (bUsePointLight) {
        bUseDirectional = false;
    }
    
    // Diffuse
    vec3 normal = normalize(localNormal);
    vec3 lightDir = bUseDirectional ? normalize(-light.worldDirection) :
    normalize(lightPosition - worldPosition);
    
    float diff = max(dot(normal, lightDir), 0.0f);
    
    vec3 diffuse = bUseTexture ? 
    diffuseColor * diff * vec3(texture(material.diffuseSampler, texCoord)) :
    diffuseColor * diff * matDiffuse;
    
    // Specular
    vec3 viewDir = normalize(viewPosition - worldPosition);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
    vec3 specular = bUseTexture ? 
    specularColor * spec * vec3(texture(material.specularSampler, texCoord)) :
    specularColor * spec * matSpecular;
    
    // Spotlight (soft edges) which requires Pointlight, emitting from camera
    if (bUseSpotLight) {
        float theta = dot(lightDir, normalize(-light.direction));
        float epsilon = (light.cutOff - light.outerCutOff);
        float intensity = clamp( (theta - light.outerCutOff) / epsilon, 0.0f, 1.0f);
    
        diffuse  *= intensity;
        specular *= intensity;
    }
    
    // Pointlight, emitting from lamp  
    if (bUsePointLight) {
        // Attenuation
        float dist = length(lightPosition - worldPosition);
        float attenuation = 1.0f / (light.constant + light.linear * dist + light.quadratic * (dist * dist));
        ambient  *= attenuation;
        diffuse  *= attenuation;
        specular *= attenuation;
    }
    
    vec3 result = (ambient + diffuse + specular) ;
    
    fOutputColor = vec4( result, 1.0f);
    
}
