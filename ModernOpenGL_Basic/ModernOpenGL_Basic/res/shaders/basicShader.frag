#version 410 core


in vec3 normal0;			// Interpolated colour using colour calculated in the vertex shader
in vec2 texCoord0;			// Interpolated texture coordinate using texture coordinate from the vertex shader

uniform mat4 MVP;
uniform mat4 Normal;
uniform sampler2D sampler;  // The texture sampler
uniform bool bUseTexture;    // A flag indicating if texture-mapping should be applied
uniform vec3 lightDirection;

out vec4 FragColor;		// The output colour

void main() {
    
    vec4 vTexColour = texture(sampler, texCoord0); 
    
    if (bUseTexture){
        
        float intensity =  dot( -lightDirection, normal0.rgb);//or//dot(-lightDirection,normalize(normal0));
        
        // Getting the light direction of the normal 
        FragColor = vTexColour * clamp( intensity, 0.0f, 1.0f);// the dot product between two vectors, if they are normalised	then it will result in the cosine between them. the reason th light direction is negative is to simulate the ligth reflecting off a surface because it is actually surposed to be the cosine of the reflected direction and normal. but this is not the actual reflection direction, however it is a nice way to simulate it. clamp allows us to not have negative lighting.
    }else{
        FragColor = vec4(normal0, 1.0f);	// Just use the colour instead
    }
}
