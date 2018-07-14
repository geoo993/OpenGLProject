#version 410 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inTexCoord;
layout (location = 2) in vec3 inNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 vPosition;
out vec2 vTexCoord;
out vec3 vNormal;

void main() {
    
    gl_Position = projection * view * model * vec4(inPosition, 1.0f);
    
    vPosition = vec3(model * vec4(inPosition, 1.0f));
    
    vTexCoord = inTexCoord;
    
    //vNormal = vec3(model * vec4(inNormal, 0.0f));
    //vNormal = (model * vec4(inNormal, 0.0)).xyz;
    
    vNormal = mat3(transpose(inverse(model))) * inNormal;
}
