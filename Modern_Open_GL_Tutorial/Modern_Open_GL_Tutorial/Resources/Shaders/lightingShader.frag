#version 410 core

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
//
//// Calculates the color when using a directional light.
//vec3 CalculateDirectionalLight( DirectionalLight light, vec3 normal, vec3 viewDirection){
//    
//    // Diffuse shading
//    vec3 lightDirection = normalize(-light.worldDirection);
//    float diff = max( dot( normal, lightDirection ), 0.0f);
//    
//    // Specular shading
//    vec3 reflectDirection = reflect(-lightDirection, normal);
//    float spec = pow( max( dot( viewDirection, reflectDirection ), 0.0f), material.shininess);
//    
//     // Combine results
//    // Combine results
//    vec3 ambient = vec3(0.0f);
//    vec3 diffuse = vec3(0.0f);
//    vec3 specular = vec3(0.0f);
//    
//    vec3 diffuseColor = light.ambient * light.base.color ;
//    vec3 ambientColor = bUseDark ? light.diffuse * diffuseColor : light.diffuse ;
//    vec3 specularColor = light.specular * light.base.color;
//    
//    if (bUseTexture) {
//        ambient = ambientColor * vec3( texture( material.diffuseSampler, texCoord ) );
//        diffuse = diffuseColor * diff * vec3( texture( material.diffuseSampler, texCoord ) );
//        specular = specularColor * spec * vec3( texture( material.specularSampler, texCoord ) );
//    }else{
//        ambient = ambientColor * material.ambient;
//        diffuse = diffuseColor * diff * material.diffuse;
//        specular = specularColor * spec * material.specular;
//    }
//    
//    return ( ambient + diffuse + specular );
//}
//
//// Pointlight, emitting from lamp  
//vec3 CalculatePointLight( PointLight light, vec3 normal, vec3 fragPosition, vec3 viewDirection, bool useViewPosition){
//    
//    // if true, we wicth to camera position instead of light position
//    vec3 lightPosition = useViewPosition ? viewPosition : light.position;
//    
//    // Diffuse shading
//    vec3 lightDirection = normalize(lightPosition - fragPosition);
//    
//    float diff = max( dot( normal, lightDirection ), 0.0f);
//    
//    // Specular shading
//    vec3 reflectDirection = reflect(-lightDirection, normal);
//    float spec = pow( max( dot( viewDirection, reflectDirection ), 0.0f), material.shininess);
//    
//    // Combine results
//    // Combine results
//    vec3 ambient = vec3(0.0f);
//    vec3 diffuse = vec3(0.0f);
//    vec3 specular = vec3(0.0f);
//    
//    vec3 diffuseColor = light.ambient * light.base.color ;
//    vec3 ambientColor = bUseDark ? light.diffuse * diffuseColor : light.diffuse ;
//    vec3 specularColor = light.specular * light.base.color;
//    
//    if (bUseTexture) {
//        ambient = ambientColor * vec3( texture( material.diffuseSampler, texCoord ) );
//        diffuse = diffuseColor * diff * vec3( texture( material.diffuseSampler, texCoord ) );
//        specular = specularColor * spec * vec3( texture( material.specularSampler, texCoord ) );
//    }else{
//        ambient = ambientColor * material.ambient;
//        diffuse = diffuseColor * diff * material.diffuse;
//        specular = specularColor * spec * material.specular;
//    }
//    
//    // Attenuation
//    float dist = length(lightPosition - fragPosition);
//    float attenuation = 1.0f / (light.constant + light.linear * dist + light.quadratic * (dist * dist));
//    ambient  *= attenuation;
//    diffuse  *= attenuation;
//    specular *= attenuation;
//    
//    return ( ambient + diffuse + specular );
//    
//}
//
// Spotlight (soft edges) which requires Pointlight, emitting from camera
vec3 CalculateSpotLight( SpotLight light, vec3 normal, vec3 fragPosition, vec3 viewDirection, bool useViewPosition)
{
    
//    float theta = dot(lightDir, normalize(-light.direction));
//    float epsilon = (light.cutOff - light.outerCutOff);
//    float intensity = clamp( (theta - light.outerCutOff) / epsilon, 0.0f, 1.0f);
//    
//    diffuse  *= intensity;
//    specular *= intensity;
    
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
    
    vec3 diffuseColor = light.ambient * directionallight.base.color ;
    vec3 ambientColor = bUseDark ? light.diffuse * diffuseColor : light.diffuse ;
    vec3 specularColor = light.specular * directionallight.base.color;
    
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




void AllTogether(SpotLight light) {
    
    if (bUseDirectional) {
        bUseDark = true;
    }
    vec3 diffuseColor = light.ambient * directionallight.base.color ;
    vec3 ambientColor = bUseDark ? light.diffuse * diffuseColor : light.diffuse ;
    vec3 specularColor = light.specular * directionallight.base.color;
    
    vec3 matAmbient = material.ambient;
    vec3 matDiffuse = material.diffuse;
    vec3 matSpecular = material.specular;
    
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
    vec3 lightDir = bUseDirectional ? normalize(-light.direction) :
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
    
    // Pointlight, emitting from lamp  
    if (bUsePointLight) {
        // Attenuation
        float dist = length(lightPosition - worldPosition);
        float attenuation = 1.0f / (light.constant + light.linear * dist + light.quadratic * (dist * dist));
        ambient  *= attenuation;
        diffuse  *= attenuation;
        specular *= attenuation;
    }
    
    // Spotlight (soft edges) which requires Pointlight, emitting from camera
    if (bUseSpotLight) {
        float theta = dot(lightDir, normalize(-light.direction));
        float epsilon = (light.cutOff - light.outerCutOff);
        float intensity = clamp( (theta - light.outerCutOff) / epsilon, 0.0f, 1.0f);
        
        diffuse  *= intensity;
        specular *= intensity;
    }
    
    vec3 result = (ambient + diffuse + specular) ;
    
    fOutputColor = vec4( result, 1.0f);
}

void main() {
    
    /*
    vec3 normal = normalize(localNormal);
    vec3 viewDirection = normalize(viewPosition - worldPosition);
    
    vec3 result = CalculateDirectionalLight( directionallight, normal, viewDirection);
    
    for (int i = 0; i < NUMBER_OF_POINT_LIGHTS; i++){
     
        pointLights[i].base = pointLights[0].base;
        pointLights[i].ambient = pointLights[0].ambient;
        pointLights[i].diffuse = pointLights[0].diffuse;
        pointLights[i].specular = pointLights[0].specular;
         
        pointLights[i].constant = pointLights[0].constant;
        pointLights[i].linear = pointLights[0].linear;
        pointLights[i].quadratic = pointLights[0].quadratic;
        
        result += CalculatePointLight(pointLights[i], normal, worldPosition, viewDirection);
    }
    
    result += CalculateSpotLight( spotLight, normal, worldPosition, viewDirection)
    //fOutputColor = vec4( result, 1.0f);
    */
    
    //AllTogether(spotlight);
    
    vec3 normal = normalize(localNormal);
    vec3 viewDirection = normalize(viewPosition - worldPosition);
    vec3 finalColor = CalculateSpotLight( spotlight, normal, worldPosition, viewDirection, true);
    
    fOutputColor = vec4( finalColor, 1.0f);
    
}
