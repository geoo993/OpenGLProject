#version 410 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inTexCoord;
layout (location = 2) in vec3 inNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 worldPosition;
out vec2 texCoord;
out vec3 localNormal;


void main() {
    
    gl_Position = projection * view * model * vec4(inPosition, 1.0f);
    
    worldPosition = vec3(model * vec4(inPosition, 1.0f));
    
    texCoord = inTexCoord;
    
    localNormal = mat3(transpose(inverse(model))) * inNormal;
    
}
