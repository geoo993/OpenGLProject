#version 410 core


//// Layout of vertex attributes in VBO
layout (location = 0) in vec3 inPosition;/////must
layout (location = 1) in vec2 inTexCoord;/////must
layout (location = 2) in vec3 inNormal;/////must

uniform mat4 MVP;
uniform sampler2D sampler;  // The texture sampler
uniform bool bUseTexture;    // A flag indicating if texture-mapping should be applied
uniform vec3 vDiffuseColor;
uniform float fTwist;

// Vertex colour output to fragment shader -- using Gouraud (interpolated) shading
out vec3 color0;	// Colour computed using reflectance model
out vec2 texCoord0;	// Texture coordinate

void main() {
    
    float angle = fTwist * length(inTexCoord.xy);
    float s = sin(angle);
    float c = cos(angle);
    vec4 twister = vec4(c * inTexCoord.x - s * inTexCoord.y,s * inTexCoord.x + c * inTexCoord.y, 0.0,1.0); 
    
    gl_Position = MVP * twister;
    
    // Pass through the texture coordinate
    texCoord0 = inTexCoord;
    
    // Apply the Phong model to compute the vertex colour
    color0 = inNormal;
    
    
}
