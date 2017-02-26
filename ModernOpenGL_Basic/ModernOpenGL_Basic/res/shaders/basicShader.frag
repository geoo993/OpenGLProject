#version 410 core


in vec3 vColour;			// Interpolated colour using colour calculated in the vertex shader
in vec2 vTexCoord0;			// Interpolated texture coordinate using texture coordinate from the vertex shader

out vec4 FragColor;		// The output colour

uniform mat4 transform;
uniform sampler2D diffuse;  // The texture sampler
uniform bool bUseTexture;    // A flag indicating if texture-mapping should be applied

void main() {
    
    // Get the texel colour from the texture sampler
    vec4 vTexColour = texture(diffuse, vTexCoord0);	
    
    if (bUseTexture)
        FragColor = vTexColour * vec4(vColour, 1.0f);	// Combine object colour and texture 
    else
        FragColor = vec4(vColour, 1.0f);	// Just use the colour instead
}
