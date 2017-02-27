#version 410 core


//// Layout of vertex attributes in VBO
layout (location = 0) in vec3 inPosition;/////must
layout (location = 1) in vec2 inTexCoord;/////must
layout (location = 2) in vec3 inNormal;/////must

uniform mat4 MVP;
uniform sampler2D sampler;  // The texture sampler
uniform bool bUseTexture;    // A flag indicating if texture-mapping should be applied

// Vertex colour output to fragment shader -- using Gouraud (interpolated) shading
out vec3 color0;	// Colour computed using reflectance model
out vec2 texCoord0;	// Texture coordinate

void main() {
    
    gl_Position = MVP * vec4(inPosition, 1.0);
    
    // Pass through the texture coordinate
    texCoord0 = inTexCoord;
    
    // Apply the Phong model to compute the vertex colour
    color0 = inNormal;
    
    
}
