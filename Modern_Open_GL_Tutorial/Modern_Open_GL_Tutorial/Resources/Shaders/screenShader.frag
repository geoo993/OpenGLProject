#version 410 core

struct Material
{
    sampler2D sampler;
};
uniform Material material;
uniform bool bUseTexture;    // A flag indicating if texture-mapping should be applied

in vec2 texCoord;
in vec3 localNormal;

out vec4 fOutputColor;		// The output colour

void main() {
    
    vec4 vTexColour = texture(material.sampler, texCoord); 
    
    if (bUseTexture){
        fOutputColor = vTexColour;
    }else{
        fOutputColor = vec4(localNormal, 1.0f);
    }
    
}
