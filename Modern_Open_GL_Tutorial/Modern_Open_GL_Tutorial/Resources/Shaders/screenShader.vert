#version 410 core

//// Layout of vertex attributes in VBO
layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inTexCoord;
layout (location = 2) in vec3 inNormal;

out vec2 texCoord;
out vec3 localNormal;
uniform mat4 MVP;

void main() {
    
    //gl_Position = vec4(inPosition.x, inPosition.y, inPosition.z, 1.0f);
    gl_Position = MVP * vec4(inPosition, 1.0);
    
    // Pass through the texture coordinate
    texCoord =  vec2(inTexCoord.x, inTexCoord.y);
    
    // Apply the Phong model to compute the vertex colour
    localNormal = inNormal;
    
    
}
