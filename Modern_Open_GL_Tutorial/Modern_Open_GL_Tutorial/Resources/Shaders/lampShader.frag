#version 410 core

struct BaseLight
{
    vec3 color;
    float intensity;
    float power;
};

uniform BaseLight light;

out vec4 fOutputColor;        // The output colour

void main() {
    
    fOutputColor = vec4(light.color, 1.0f);
    
}
