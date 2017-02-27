#version 410 core

uniform mat4 MVP;
uniform sampler2D sampler;  // The texture sampler
uniform bool bUseTexture;    // A flag indicating if texture-mapping should be applied
uniform vec3 vDiffuseColor;
uniform float fTwist;

in vec3 color0;			// Interpolated colour using colour calculated in the vertex shader
in vec2 texCoord0;			// Interpolated texture coordinate using texture coordinate from the vertex shader

out vec4 FragColor;		// The output colour

void main() {
    
    vec4 texColour = texture(sampler, texCoord0); 
    
    
    if (bUseTexture){
        
        FragColor = texColour * vec4(vDiffuseColor, 1); //* vec4(color0, 1.0f);
    }else{
        FragColor = vec4(color0, 1.0f);	// Just use the colour instead
    }
    
}
