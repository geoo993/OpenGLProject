#version 410 core

struct Material
{
    sampler2D sampler;
    sampler2D normalSampler;
    sampler2D diffuseSampler;
    sampler2D specularSampler;
    
    float shininess;
    float intensity;
    
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
    Attenuation atten;
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
uniform DirectionalLight R_directionallight;
uniform PointLight R_pointlight;
uniform SpotLight R_spotlight;

uniform vec3 viewPosition;
uniform bool bUseTexture;

vec4 CalcLight(BaseLight base, vec3 direction, vec3 normal, vec3 worldPosition)
{
    float diffuseFactor = dot(normal, -direction);
    
    vec4 diffuseColor = vec4(0.0f,0.0f,0.0f,0.0f);
    vec4 specularColor = vec4(0.0f,0.0f,0.0f,0.0f);
    
    if(diffuseFactor > 0.0f)
    {
        diffuseColor = vec4(base.color, 1.0f) * base.intensity * diffuseFactor;
        
        vec3 directionToEye = normalize(viewPosition - worldPosition); // viewDirection
        //vec3 reflectDirection = normalize(reflect(direction, normal));
        vec3 halfDirection = normalize(directionToEye - direction);
        
        float specularFactor = dot(halfDirection, normal);
        //float specularFactor = dot(directionToEye, reflectDirection);
        specularFactor = pow(specularFactor, material.shininess);
        
        if(specularFactor > 0.0f)
        {
            specularColor = vec4(base.color, 1.0f) * material.intensity * specularFactor;
        }
    }
    
    return diffuseColor + specularColor;
}

vec4 CalcPointLight(PointLight pointLight, vec3 normal, vec3 worldPosition)
{
    vec3 lightDirection = worldPosition - pointLight.position;
    float distanceToPoint = length(lightDirection);
    
    if(distanceToPoint > pointLight.range)
        return vec4(0.0f, 0.0f, 0.0f, 0.0f);
    
    lightDirection = normalize(lightDirection);
    
    vec4 color = CalcLight(pointLight.base, lightDirection, normal, worldPosition);
    
    // Attenuation
    float attenuation = 
    pointLight.atten.constant +
    pointLight.atten.linear * 
    distanceToPoint +
    pointLight.atten.exponent * 
    distanceToPoint * 
    distanceToPoint +
    0.0001f;
    
    return color / attenuation;
}

vec4 CalcSpotLight(SpotLight spotLight, vec3 normal, vec3 worldPosition)
{
    vec3 lightDirection = normalize(worldPosition - spotLight.pointLight.position);
    
    // Intensity
    float spotFactor = dot(lightDirection, spotLight.direction);
    
    vec4 color = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    
    if(spotFactor > spotLight.cutoff)
    {
        color = CalcPointLight(spotLight.pointLight, normal, worldPosition) *
        (1.0f - (1.0f - spotFactor) / (1.0f - spotLight.cutoff));
    }
    
    return color;
}

vec4 CalcDirectionalLight(DirectionalLight directionalLight, vec3 normal, vec3 worldPosition)
{
    return CalcLight(directionalLight.base, -directionalLight.direction, normal, worldPosition);
}



//vec4 CalcLightingEffect(vec3 normal, vec3 worldPosition)
//{
//    return CalcDirectionalLight(R_directionallight, normal, worldPosition);
//}


vec4 CalcLightingEffect(vec3 normal, vec3 worldPosition)
{
    return CalcPointLight(R_pointlight, normal, worldPosition);
}

//vec4 CalcLightingEffect(vec3 normal, vec3 worldPosition)
//{
//    return CalcSpotLight(R_spotlight, normal, worldPosition);
//}

in vec3 vWorldPosition;
in vec2 vTexCoord;
in vec3 vLocalNormal;

out vec4 fOutputColor;        // The output colour


void main() {
    
    vec4 texColor = texture( material.diffuseSampler, vTexCoord );
    fOutputColor = texColor * CalcLightingEffect(normalize(vLocalNormal), vWorldPosition);
    
    //fOutputColor = vec4( 1.0f);
}
