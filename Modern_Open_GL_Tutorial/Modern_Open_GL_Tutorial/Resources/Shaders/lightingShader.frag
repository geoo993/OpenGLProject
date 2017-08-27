#version 410 core

struct Material
{
    sampler2D diffuseSampler;
    sampler2D specularSampler;
    float shininess;
};

struct Light
{
    vec3 position;
    
    vec3 color;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform Light light;
uniform vec3 viewPosition;
uniform bool bUseTexture;

bool bUseDark = false;

in vec3 worldPosition;
in vec2 texCoord;
in vec3 localNormal;

out vec4 fOutputColor;		// The output colour

void main() {
    
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
    
    // Diffuse
    vec3 normal = normalize(localNormal);
    vec3 lightDir = normalize(light.position - worldPosition);
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
    
    vec3 result = (ambient + diffuse + specular) ;
    
    fOutputColor = vec4( result, 1.0f);
    
}
