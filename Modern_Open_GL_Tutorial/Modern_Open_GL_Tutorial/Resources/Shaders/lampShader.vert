#version 410 core

//// Layout of vertex attributes in VBO
layout (location = 0) in vec3 inPosition;

uniform mat4 MVP;

void main() {
    
    gl_Position = MVP * vec4(inPosition, 1.0f);
    
}
