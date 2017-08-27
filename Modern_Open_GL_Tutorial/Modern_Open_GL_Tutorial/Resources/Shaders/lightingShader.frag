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


in vec3 worldPosition;
in vec2 texCoord;
in vec3 localNormal;

out vec4 fOutputColor;		// The output colour

void main() {
    
    // Ambient
    vec3 ambient = light.ambient * vec3(texture(material.diffuseSampler, texCoord));
    
    // Diffuse
    vec3 norm = normalize(localNormal);
    vec3 lightDir = normalize(light.position - worldPosition);
    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuseSampler, texCoord));
    
    // Specular
    vec3 viewDir = normalize(viewPosition - worldPosition);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specularSampler, texCoord));
    
    vec3 result = (ambient + diffuse + specular); //* light.color;
    
    fOutputColor = vec4(result, 1.0f);
    
}
