#version 410 core

uniform mat4 MVP;
uniform sampler2D sampler;  // The texture sampler
uniform bool bUseTexture;    // A flag indicating if texture-mapping should be applied

in vec3 color0;			// Interpolated colour using colour calculated in the vertex shader
in vec2 texCoord0;			// Interpolated texture coordinate using texture coordinate from the vertex shader

out vec4 FragColor;		// The output colour

void main() {
    
    vec4 vTexColour = texture(sampler, texCoord0); 
    
    if (bUseTexture){
        
        FragColor = vTexColour; //* vec4(color0, 1.0f);
    }else{
        FragColor = vec4(color0, 1.0f);	// Just use the colour instead
    }
    
}
