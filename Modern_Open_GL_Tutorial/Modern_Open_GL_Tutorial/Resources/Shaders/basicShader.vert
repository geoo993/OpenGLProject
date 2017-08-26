#version 410 core

//// Layout of vertex attributes in VBO
layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inTexCoord;
layout (location = 2) in vec3 inNormal;

uniform mat4 MVP;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// Vertex colour output to fragment shader -- using Gouraud (interpolated) shading
out vec3 colour;	// Colour computed using reflectance model
out vec3 normal;	// Normal computed using reflectance model
out vec2 texCoord;	// Texture coordinate

void main() {
    
    gl_Position = MVP * vec4(inPosition, 1.0f);
    
    // Pass through the texture coordinate
    texCoord = inTexCoord;
    
    // Apply lighting to the model
    normal = (model * vec4(inNormal, 0.0)).xyz;//swizzerling
    
    // Apply the Phong model to compute the vertex colour
    colour = inNormal;
    
    
}
