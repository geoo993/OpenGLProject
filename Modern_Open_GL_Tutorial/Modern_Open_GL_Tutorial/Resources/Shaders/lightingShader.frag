#version 410 core

// from https://www.youtube.com/watch?v=vnUQOIODAKc&index=25&list=PLRtjMdoYXLf6zUMDJVRZYV-6g6n62vet8


#define NUMBER_OF_POINT_LIGHTS 4

struct Material
{
    sampler2D sampler;
    sampler2D normalSampler;
    sampler2D diffuseSampler;
    sampler2D specularSampler;
    float shininess;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct BaseLight
{
    vec3 color;
    float intensity;
};

struct DirectionalLight
{
    BaseLight base;
    vec3 worldDirection;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
};

struct PointLight
{
    BaseLight base;
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
    PointLight pointlight;
    
    vec3 position;
    vec3 direction;
    
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
uniform DirectionalLight directionallight;
uniform PointLight pointlights[NUMBER_OF_POINT_LIGHTS];
uniform SpotLight spotlight;

uniform vec3 viewPosition;
uniform bool bUseTexture;

in vec3 worldPosition;
in vec2 texCoord;
in vec3 localNormal;

out vec4 fOutputColor;		// The output colour

bool bUseDark = false;

bool bUseDirectional = false;  ///directional light else basic lamp illumination

bool bUsePointLight = true; //point light, uses Attenuation and illuminates objects closer to lamp

bool bUseSpotLight = true; //spot light, emitting light from camera

// Calculates the color when using a directional light.
vec3 CalculateDirectionalLight( DirectionalLight light, vec3 normal, vec3 viewDirection){
    
    // Diffuse shading
    vec3 lightDirection = normalize(-light.worldDirection);
    float diff = max( dot( normal, lightDirection ), 0.0f);
    
    // Specular shading
    vec3 reflectDirection = reflect(-lightDirection, normal);
    float spec = pow( max( dot( viewDirection, reflectDirection ), 0.0f), material.shininess);
    
    // Combine results
    vec3 ambient = vec3(0.0f);
    vec3 diffuse = vec3(0.0f);
    vec3 specular = vec3(0.0f);
    
    vec3 diffuseColor = light.ambient * light.base.color ;
    vec3 ambientColor = bUseDark ? light.diffuse * diffuseColor : light.diffuse ;
    vec3 specularColor = light.specular * light.base.color;
    
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


// Pointlight, emitting from lamp  
vec3 CalculatePointLight( PointLight light, vec3 normal, vec3 fragPosition, vec3 viewDirection, bool useViewPosition){
    
    // if true, we wicth to camera position instead of light position
    vec3 lightPosition = useViewPosition ? viewPosition : light.position;
    
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
    
    vec3 diffuseColor = light.ambient * light.base.color ;
    vec3 ambientColor = bUseDark ? light.diffuse * diffuseColor : light.diffuse ;
    vec3 specularColor = light.specular * light.base.color;
    
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
vec3 CalculateSpotLight( SpotLight light, vec3 normal, vec3 fragPosition, vec3 viewDirection, bool useViewPosition)
{
    
//    float theta = dot(lightDir, normalize(-light.direction));
//    float epsilon = (light.cutOff - light.outerCutOff);
//    float intensity = clamp( (theta - light.outerCutOff) / epsilon, 0.0f, 1.0f);
//    
//    diffuse  *= intensity;
//    specular *= intensity;
    
    vec3 lColor = vec3(1.0f, 1.0f, 1.0f);
    
    // if true, we wicth to camera position instead of light position
    vec3 lightPosition = useViewPosition ? viewPosition : light.position;
    
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
    
    vec3 diffuseColor = light.ambient * lColor ;
    vec3 ambientColor = bUseDark ? light.diffuse * diffuseColor : light.diffuse ;
    vec3 specularColor = light.specular * lColor;
    
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
    vec3 viewDirection = normalize(viewPosition - worldPosition);
    
    vec3 result = CalculateDirectionalLight( directionallight, normal, viewDirection);
    
    for (int i = 0; i < NUMBER_OF_POINT_LIGHTS; i++){
     
       
        PointLight firstpl = pointlights[0];
        PointLight pl = pointlights[i];
        
        pl.base = firstpl.base;
        pl.ambient = firstpl.ambient;
        pl.diffuse = firstpl.diffuse;
        pl.specular = firstpl.specular;
                 
        pl.constant = firstpl.constant;
        pl.linear = firstpl.linear;
        pl.quadratic = firstpl.quadratic;
                
        result += CalculatePointLight(pl, normal, worldPosition, viewDirection, false);
    }
    
    
    
    vec3 spotl = CalculateSpotLight( spotlight, normal, worldPosition, viewDirection, true);
    result += spotl;
    
    fOutputColor = vec4( result, 1.0f);
}
