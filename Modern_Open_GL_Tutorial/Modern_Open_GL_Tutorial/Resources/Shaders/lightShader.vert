#version 410 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inTexCoord;
layout (location = 2) in vec3 inNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 vWorldPosition;
out vec2 vTexCoord;
out vec3 vLocalNormal;

void main() {
    
    gl_Position = projection * view * model * vec4(inPosition, 1.0f);
    
    vWorldPosition = vec3(model * vec4(inPosition, 1.0f));
    
    vTexCoord = inTexCoord;
    
    //vLocalNormal = vec3(model * vec4(inNormal, 0.0f));
    //vLocalNormal = (model * vec4(inNormal, 0.0)).xyz;
    
    vLocalNormal = mat3(transpose(inverse(model))) * inNormal;
}
