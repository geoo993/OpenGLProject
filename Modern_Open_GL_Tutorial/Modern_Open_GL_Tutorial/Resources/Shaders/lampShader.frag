#version 410 core

out vec4 fOutputColor;		// The output colour

struct Light
{
    vec3 color;
};
uniform Light light;

void main() {
    
    fOutputColor = vec4(light.color, 1.0f);
    
}
