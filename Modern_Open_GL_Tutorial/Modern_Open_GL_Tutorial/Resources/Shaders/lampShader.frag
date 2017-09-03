#version 410 core

uniform vec3 lightColor;

out vec4 fOutputColor;        // The output colour

void main() {
    
    fOutputColor = vec4(lightColor, 1.0f);
    
}
