#version 410 core
// https://github.com/BennyQBD/3DGameEngineCpp_60/tree/master/3DEngineCpp/res/shaders
// https://github.com/BennyQBD/3DGameEngineCpp_60/blob/master/3DEngineCpp/res/shaders/lighting.glh

#define NUMBER_OF_DIRECTIONAL_LIGHTS 1
#define NUMBER_OF_POINT_LIGHTS 5


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
    float outerCutoff;
};

uniform Material material;
uniform DirectionalLight R_directionallight[NUMBER_OF_DIRECTIONAL_LIGHTS];
uniform PointLight R_pointlight[NUMBER_OF_POINT_LIGHTS];
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
        //float epsilon = (1.0f - spotLight.cutoff);
        //float intensity = (1.0f - (1.0f - spotFactor) / epsilon);
        
        float epsilon = (spotLight.cutoff - spotLight.outerCutoff );
        float intensity = clamp( ( spotLight.outerCutoff - spotFactor ) / epsilon, 0.0f, 1.0f);
        
        color = CalcPointLight(spotLight.pointLight, normal, worldPosition) * intensity;
    }
    
    return color;
}

vec4 CalcDirectionalLight(DirectionalLight directionalLight, vec3 normal, vec3 worldPosition)
{
    return CalcLight(directionalLight.base, -directionalLight.direction, normal, worldPosition);
}

//vec4 CalcLightingEffect(int type, vec3 normal, vec3 worldPosition)
//{
//    if (type == 0)
//        return CalcDirectionalLight(R_directionallight, normal, worldPosition);
//    
//    if (type == 1)
//        return CalcPointLight(R_pointlight, normal, worldPosition);
//
//    if (type == 2)
//        return CalcSpotLight(R_spotlight, normal, worldPosition);
//}

in vec3 vWorldPosition;
in vec2 vTexCoord;
in vec3 vLocalNormal;

out vec4 fOutputColor;        // The output colour


void main() {
    
    //Combining Lights
    
    // Properties
    vec4 texColor = texture( material.diffuseSampler, vTexCoord );
    vec3 normal = normalize(vLocalNormal);
    vec4 result = vec4(0.0f);
    
    // Directional lighting
    for (int i = 0; i < NUMBER_OF_DIRECTIONAL_LIGHTS; i++){
        vec4 directionalLight = CalcDirectionalLight(R_directionallight[i], normal, vWorldPosition);
        result += directionalLight;
    }
    
    // Point lights
    //vec4 pointL = CalcPointLight(R_pointlight1, normal, vWorldPosition);
    //result += pointL;
    
    for (int i = 0; i < NUMBER_OF_POINT_LIGHTS; i++){
        vec4 pointL = CalcPointLight(R_pointlight[i], normal, vWorldPosition);
        result += pointL;
    }

    
    // Spot light
    vec4 spotL = CalcSpotLight(R_spotlight, normal, vWorldPosition);
    result += spotL;
    
    fOutputColor = texColor * result;
}
