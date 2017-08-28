#version 410 core

struct Material
{
    sampler2D sampler;
};
uniform Material material;
uniform bool bUseTexture;    // A flag indicating if texture-mapping should be applied

in vec3 colour;	
in vec3 normal;	
in vec2 texCoord;

out vec4 fOutputColor;		// The output colour

void main() {
    
    vec4 vTexColour = texture(material.sampler, texCoord); 
    
    if (bUseTexture){
        fOutputColor = vTexColour;
    }else{
        fOutputColor = vec4(colour, 1.0f);	// Just use the colour instead
    }
    
}
