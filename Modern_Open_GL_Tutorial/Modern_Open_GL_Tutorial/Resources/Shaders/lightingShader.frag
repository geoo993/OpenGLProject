#version 410 core

// from https://www.youtube.com/watch?v=vnUQOIODAKc&index=25&list=PLRtjMdoYXLf6zUMDJVRZYV-6g6n62vet8

struct Material
{
    sampler2D sampler;
    sampler2D normalSampler;
    sampler2D diffuseSampler;
    sampler2D specularSampler;
    float shininess;
    float intensity;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct BaseLight
{
    vec3 color;
    float intensity;

    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
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
    Attenuation atten;
    vec3 position;
    float range;
};


struct SpotLight
{
    PointLight pointLight;
    vec3 direction;
    
    float cutOff;
    float outerCutOff;
};

uniform Material material;
uniform DirectionalLight directionallight;
uniform PointLight pointlight;
uniform SpotLight spotlight;

uniform vec3 cameraPosition;
uniform bool bUseTexture;
uniform bool bUseDirectionalLight;
uniform bool bUsePointLight;
uniform bool bUseSpotlight;

in vec3 worldPosition;
in vec2 texCoord;
in vec3 localNormal;

out vec4 fOutputColor;		// The output colour

bool bUseDark = false;

//bool bUseDirectional = false;  ///directional light else basic lamp illumination

//bool bUsePointLight = true; //point light, uses Attenuation and illuminates objects closer to lamp

//bool bUseSpotLight = true; //spot light, emitting light from camera

// Calculates the color when using a directional light.
vec3 CalculateDirectionalLight( DirectionalLight light, vec3 normal, vec3 viewDirection){
    
    vec3 lightColor = light.base.color;
    
    // Diffuse shading
    vec3 lightDirection = normalize(-light.direction);
    float diff = max( dot( normal, lightDirection ), 0.0f);
    
    // Specular shading
    vec3 reflectDirection = reflect(-lightDirection, normal);
    float spec = pow( max( dot( viewDirection, reflectDirection ), 0.0f), material.shininess);
    
    // Combine results
    vec3 ambient = vec3(0.0f);
    vec3 diffuse = vec3(0.0f);
    vec3 specular = vec3(0.0f);
    
    vec3 diffuseColor = light.ambient * lightColor ;
    vec3 ambientColor = bUseDark ? light.diffuse * diffuseColor : light.diffuse ;
    vec3 specularColor = light.specular * lightColor;
    
    if (bUseTexture) {
        ambient = ambientColor * vec3( texture( material.diffuseSampler, texCoord ) );
        diffuse = diffuseColor * diff * vec3( texture( material.diffuseSampler, texCoord ) );
        specular = specularColor * spec * vec3( texture( material.specularSampler, texCoord ) );
    }else{
        ambient = ambientColor * material.ambient;
        diffuse = diffuseColor * diff * material.diffuse;
        specular = specularColor * spec * material.specular;
    }
    
    return ( ambient + diffuse + specular );
}


// Calculates the color when using a point light, emitting from lamp  
vec3 CalculatePointLight( PointLight light, vec3 normal, vec3 fragPosition, vec3 viewDirection){
    
    vec3 lightColor = light.base.color;
    
    vec3 lightPosition = light.position;
    
    // Diffuse shading
    vec3 lightDirection = normalize(lightPosition - fragPosition);
    
    float diff = max( dot( normal, lightDirection ), 0.0f);
    
    // Specular shading
    vec3 reflectDirection = reflect(-lightDirection, normal);
    float spec = pow( max( dot( viewDirection, reflectDirection ), 0.0f), material.shininess);
    
    // Combine results
    vec3 ambient = vec3(0.0f);
    vec3 diffuse = vec3(0.0f);
    vec3 specular = vec3(0.0f);
    
    vec3 diffuseColor = light.ambient * lightColor ;
    vec3 ambientColor = bUseDark ? light.diffuse * diffuseColor : light.diffuse ;
    vec3 specularColor = light.specular * lightColor;
    
    if (bUseTexture) {
        ambient = ambientColor * vec3( texture( material.diffuseSampler, texCoord ) );
        diffuse = diffuseColor * diff * vec3( texture( material.diffuseSampler, texCoord ) );
        specular = specularColor * spec * vec3( texture( material.specularSampler, texCoord ) );
    }else{
        ambient = ambientColor * material.ambient;
        diffuse = diffuseColor * diff * material.diffuse;
        specular = specularColor * spec * material.specular;
    }
    
    // Attenuation
    float dist = length(lightPosition - fragPosition);
    float attenuation = 1.0f / (light.constant + light.linear * dist + light.quadratic * (dist * dist));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    
    return ( ambient + diffuse + specular );
    
}

// Spotlight (soft edges) which requires Pointlight, emitting from camera
vec3 CalculateSpotLight( SpotLight light, vec3 normal, vec3 fragPosition, vec3 viewDirection)
{
   
    vec3 lightColor = light.base.color;
    
    // if true, we wicth to camera position instead of light position
    vec3 lightPosition = light.position; 
    
    // Diffuse shading
    vec3 lightDirection = normalize(lightPosition - fragPosition);
    
    float diff = max( dot( normal, lightDirection ), 0.0f);
    
    // Specular shading
    // r or the reflection vector, is the normalised reflection vector
    vec3 reflectDirection = reflect(-lightDirection, normal);
    float spec = pow( max( dot( viewDirection, reflectDirection ), 0.0f), material.shininess);
    
    // Combine results
    vec3 ambient = vec3(0.0f);
    vec3 diffuse = vec3(0.0f);
    vec3 specular = vec3(0.0f);
    
    vec3 diffuseColor = light.ambient * lightColor ;
    vec3 ambientColor = bUseDark ? light.diffuse * diffuseColor : light.diffuse ;
    vec3 specularColor = light.specular * lightColor;
    
    if (bUseTexture) {
        ambient = ambientColor * vec3( texture( material.diffuseSampler, texCoord ) );
        diffuse = diffuseColor * diff * vec3( texture( material.diffuseSampler, texCoord ) );
        specular = specularColor * spec * vec3( texture( material.specularSampler, texCoord ) );
    }else{
        ambient = ambientColor * material.ambient;
        diffuse = diffuseColor * diff * material.diffuse;
        specular = specularColor * spec * material.specular;
    }
    
    // Attenuation
    float dist = length(lightPosition - fragPosition);
    float attenuation = 1.0f / (light.constant + light.linear * dist + light.quadratic * (dist * dist));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    
    // Intensity
    float theta = dot(lightDirection, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp( (theta - light.outerCutOff) / epsilon, 0.0f, 1.0f);
    
    diffuse  *= intensity;
    specular *= intensity;
    
    return ( ambient + diffuse + specular );
}



void main() {
    
   
    vec3 normal = normalize(localNormal);
    vec3 viewDirection = normalize(cameraPosition - worldPosition);
    
    vec3 directionalL = CalculateDirectionalLight( directionallight, normal, viewDirection);
    
    vec3 pointL = CalculatePointLight(pointlight, normal, worldPosition, viewDirection);
    
    vec3 spotL = CalculateSpotLight( spotlight, normal, worldPosition, viewDirection);
    
    fOutputColor = vec4( spotL, 1.0f);
}
