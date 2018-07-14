#version 410 core
// https://github.com/BennyQBD/3DGameEngineCpp_60/tree/master/3DEngineCpp/res/shaders
// https://github.com/BennyQBD/3DGameEngineCpp_60/blob/master/3DEngineCpp/res/shaders/lighting.glh
// https://learnopengl.com/Lighting/Light-casters
// https://learnopengl.com/Lighting/Multiple-lights

#define NUMBER_OF_DIRECTIONAL_LIGHTS 1
#define NUMBER_OF_POINT_LIGHTS 5

struct Camera
{
    vec3 position;
    vec3 front;
};

struct Material
{
    sampler2D sampler;
    sampler2D normalSampler;
    sampler2D diffuseSampler;
    sampler2D specularSampler;
    
    float shininess;
    vec3 color;

};

struct BaseLight
{
    vec3 color;
    float intensity;
    float power;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};

struct Attenuation
{
    float constant;
    float linear;
    float exponent;
};

struct DirectionalLight
{
    BaseLight base;
    vec3 direction;
};

struct PointLight
{
    BaseLight base;
    Attenuation atten;
    vec3 position;
    float range;
};

struct SpotLight
{
    PointLight pointLight;
    vec3 direction;
    float cutOff;
    float outerCutOff;
};

uniform Material material;
uniform Camera camera;
uniform DirectionalLight R_directionallight[NUMBER_OF_DIRECTIONAL_LIGHTS];
uniform PointLight R_pointlight[NUMBER_OF_POINT_LIGHTS];
uniform SpotLight R_spotlight;

uniform bool bUseTexture;
uniform bool bUseDirectionalLight;
uniform bool bUsePointLight;
uniform bool bUseSpotlight;

in vec3 vPosition;
in vec2 vTexCoord;
in vec3 vNormal;
out vec4 fOutputColor;        // The output colour

vec4 CalcLight(BaseLight base, vec3 lightDirection, vec3 viewDirection, vec3 normal)
{
    float diffuseFactor = max(dot(normal, lightDirection), 0.0f);

    vec4 diffuseColor = vec4(0.0f,0.0f,0.0f,0.0f);
    vec4 specularColor = vec4(0.0f,0.0f,0.0f,0.0f);
    vec4 ambientColor = vec4(0.0f,0.0f,0.0f,0.0f);

    if(diffuseFactor > 0.0f)
    {

        // Diffuse
        diffuseColor = base.diffuse * vec4(base.color, 1.0f) * base.intensity * diffuseFactor;

        // Ambient
        ambientColor = base.ambient * diffuseColor;

        // Specular
        vec3 reflectDirection = reflect(-lightDirection, normal);
        float specularFactor = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess);

        if(specularFactor > 0.0f)
        {
            specularColor = base.specular * vec4(base.color, 1.0f) * base.power * specularFactor;
        }
    }

    if (bUseTexture) {
        vec4 ambient = ambientColor * texture( material.diffuseSampler, vTexCoord ) ;
        vec4 diffuse = diffuseColor *  texture( material.diffuseSampler, vTexCoord ) ;
        vec4 specular = specularColor * texture( material.specularSampler, vTexCoord );

        return ambient + diffuse + specular;
    }else{
        return (ambientColor + diffuseColor + specularColor) * vec4(material.color, 1.0f);
    }
}


vec4 CalcDirectionalLight(DirectionalLight directionalLight, vec3 viewDirection, vec3 normal)
{
    return CalcLight(directionalLight.base, -directionalLight.direction, viewDirection, normal);
}

vec4 CalcPointLight(PointLight pointLight, vec3 vertexPosition, vec3 viewDirection, vec3 normal)
{
    // lightDirection also known as s,
    vec3 lightDirection = pointLight.position - vertexPosition;
    float distanceToPoint = length(lightDirection);

    //if(distanceToPoint > pointLight.range)
    //    return vec4(0.0f, 0.0f, 0.0f, 0.0f);

    vec4 color = CalcLight(pointLight.base, normalize(lightDirection), viewDirection, normal);

    // Attenuation
    ///*
    float attenuation = (
    pointLight.atten.constant +
    pointLight.atten.linear *
    distanceToPoint +
    pointLight.atten.exponent *
    (distanceToPoint * distanceToPoint) + 0.0001f);
    return color / attenuation;
    //*/

    /*
    float attenuation =
    1.0f / (pointLight.atten.constant +
            pointLight.atten.linear *
            distanceToPoint +
            pointLight.atten.exponent *
            (distanceToPoint * distanceToPoint));
    return color * attenuation;
     */
}


vec4 CalcSpotLight(SpotLight spotLight, vec3 vertexPosition, vec3 viewDirection, vec3 normal)
{
    vec3 lightDirection = normalize(spotLight.pointLight.position - vertexPosition);

    // Intensity
    //float spotFactor = dot(lightDirection, normalize(-spotLight.direction));
    float spotFactor = max(dot(lightDirection, normalize(-spotLight.direction)), 0.0f); //// no difference
    vec4 color = vec4( 0.0f, 0.0f, 0.0f, 0.0f);

    if(spotFactor > spotLight.cutOff)
    {
        float epsilon = spotLight.cutOff - spotLight.outerCutOff;
        float intensity = clamp((spotFactor - spotLight.outerCutOff) / epsilon, 0.0f, 1.0f);
        //float intensity = (1.0f - (1.0f - spotFactor) / (1.0 - spotLight.cutOff));

        color = CalcPointLight(spotLight.pointLight, vertexPosition, viewDirection, normal) * intensity;
    }

    return color;
}


void main() {
    
    //Combining Lights
    // Properties
    vec3 normal = normalize(vNormal);
    vec3 view =  camera.position + camera.front;
    vec3 directionToEye = normalize(view - vPosition); // viewDirection
    vec4 result = vec4(0.0f);


    if (bUseDirectionalLight){
        // Directional lighting
        for (int i = 0; i < NUMBER_OF_DIRECTIONAL_LIGHTS; i++){
            vec4 directionalLight = CalcDirectionalLight(R_directionallight[i], directionToEye, normal);
            result += directionalLight;
        }
    }

    if (bUsePointLight){
        // Point lights
        for (int i = 0; i < NUMBER_OF_POINT_LIGHTS; i++){
            vec4 pointL = CalcPointLight(R_pointlight[i], vPosition, directionToEye, normal);
            result += pointL;
        }
    }

    if (bUseSpotlight){
        // Spot light
        vec4 spotL = CalcSpotLight(R_spotlight, vPosition, directionToEye, normal);
        result += spotL;
    }
 
    fOutputColor = result;
}
