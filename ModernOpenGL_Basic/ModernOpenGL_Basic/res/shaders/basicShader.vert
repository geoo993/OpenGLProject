#version 410 core


//// Layout of vertex attributes in VBO
layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inTexCoord;
layout (location = 2) in vec3 inNormal;


// Vertex colour output to fragment shader -- using Gouraud (interpolated) shading
out vec3 vColour;	// Colour computed using reflectance model
out vec2 vTexCoord0;	// Texture coordinate

void main() {
    
    gl_Position = vec4(inPosition, 1.0);
    
    // Pass through the texture coordinate
    vTexCoord0 = inTexCoord;
    
    // Apply the Phong model to compute the vertex colour
    vColour = inNormal;
    
    
}