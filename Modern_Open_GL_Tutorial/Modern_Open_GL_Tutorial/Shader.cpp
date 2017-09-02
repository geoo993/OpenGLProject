//
//  Shader.cpp
//
//  Created by GEORGE QUENTIN on 25/02/2017.
//  Copyright © 2017 LEXI LABS. All rights reserved.
//

#include "Shader.h"

void Shader::Create(const std::string &fileName, 
                    const GLuint & numberOfDirectionalLights,
                    const GLuint & numberOfPointLights,
                    const GLuint & numberOfSpottLights){
    //creating a new rogram, create some space in the gpu for our shader program, 
    //open gl will give us some number to refer to it by
    //to think of it, there are actually shader files that exist on the hard drive
    //you can create, compile, attach, link and use the shader, we that during this process
    m_program = glCreateProgram();
    
    //vertex shader
    m_shaders[0] = CreateShader( LoadShader( fileName + ".vert"), GL_VERTEX_SHADER  );
    
    //fragment shader
    m_shaders[1] = CreateShader( LoadShader( fileName + ".frag"), GL_FRAGMENT_SHADER  );
    
    for (unsigned int i = 0; i < NUM_SHADERS; i++){
        glAttachShader(m_program, m_shaders[i]);
    }
    
    //tells open gl what part of data to read in your shader program
    glBindAttribLocation(m_program, 0, "inPosition");
    glBindAttribLocation(m_program, 1, "inTexCoord");
    glBindAttribLocation(m_program, 2, "inNormal");
    
    
    //we link all the attached shaders with out game
    glLinkProgram(m_program);
    CheckShaderError(m_program, GL_LINK_STATUS, true, "Error: Shader program linking failure. ");
    
    //validation, by checking shader program that it is still valid
    glValidateProgram(m_program);
    CheckShaderError(m_program, GL_VALIDATE_STATUS, true, "Error: Shader program is in-valid. ");
    
    //glGetUniformLocation takes two parameters, 
    //first it gets the program we want to get the uniform from
    //then the second is the uniform variable we want to get, it is the name of the unform variable
    m_uniforms[MVP_U] = glGetUniformLocation(m_program, "MVP");
    
    m_uniforms[MODEL_U] = glGetUniformLocation(m_program, "model");
    
    m_uniforms[VIEW_U] = glGetUniformLocation(m_program, "view");
    
    m_uniforms[PROJECTION_U] = glGetUniformLocation(m_program, "projection");
    
    m_uniforms[USETEXTURE_U]  = glGetUniformLocation(m_program, "bUseTexture");
    
    m_uniforms[VIEWPOSITION_U] = glGetUniformLocation(m_program, "viewPosition");
    
    
    // DIRECTIONAL LIGHT

    CreateDirectionalLights("R_directionallight[0]", numberOfDirectionalLights);

    // POINT LIGHT
    CreatePointLights("R_pointlight", numberOfPointLights);
    
    // SPOT LIGHT
    CreateSpotLights("R_spotlight", numberOfSpottLights);
    
    
    //Material
    m_uniforms[SAMPLER_U] = glGetUniformLocation(m_program, "material.sampler");
    
    m_uniforms[MATERIALNORMALSAMPLER_U] = glGetUniformLocation(m_program, "material.normalSampler");
    
    m_uniforms[MATERIALDIFFUSESAMPLER_U] = glGetUniformLocation(m_program, "material.diffuseSampler");
    
    m_uniforms[MATERIALSPECULARSAMPLER_U] = glGetUniformLocation(m_program, "material.specularSampler");
    
    m_uniforms[MATERIALSHININESS_U] = glGetUniformLocation(m_program, "material.shininess");
    
    m_uniforms[MATERIALINTENSITY_U] = glGetUniformLocation(m_program, "material.intensity");
    
    m_uniforms[MATERIALAMBIENT_U] = glGetUniformLocation(m_program, "material.ambient");
    
    m_uniforms[MATERIALDIFFUSE_U] = glGetUniformLocation(m_program, "material.diffuse");
    
    m_uniforms[MATERIALSPECULAR_U] = glGetUniformLocation(m_program, "material.specular");
    
}

void Shader::CreateDirectionalLights(const std::string &directionalLightName, const GLuint & numberOfDirectionalLights ){
    
    //for (unsigned int i = 0; i < numberOfDirectionalLights; ++i){
        // DIRECTIONAL LIGHT
        CreateDirectionalLightUniform(directionalLightName);
    //}
}


void Shader::CreatePointLights(const std::string &pointLightName, const GLuint & numberOfPointLights ){
    
    //for (unsigned int i = 0; i < numberOfPointLights; i++){
        // POINT LIGHT
    CreatePointLightUniform(pointLightName);
    //}
    
}


void Shader::CreateSpotLights(const std::string &spotLightName, const GLuint & numberOfSpottLights ){
    
    // SPOT LIGHT
    CreateSpotLightUniform(spotLightName);
}

Shader::~Shader(){
    
    for (unsigned int i = 0; i < NUM_SHADERS; i++){
        glDetachShader(m_program, m_shaders[i]);
        glDeleteShader(m_shaders[i]);
    }
    
    delete m_camera;
    glDeleteProgram(m_program);
}

GLuint Shader::CreateShader (const std::string &text, GLenum shadertype)
{
    
    // Create and compile the fragment shader
    GLuint shader = glCreateShader(shadertype);
    
    if (shader == 0){
        std::cerr << "Error: Shader creation failed " << std::endl;
        return 0;
    }
    
    //get the shader file string, a list of one c string
    const GLchar * shaderSource = text.c_str();
    //the length of the c string
    GLint shaderSourceLength = text.length();
    
    glShaderSource(shader, 1, &shaderSource, &shaderSourceLength);
    glCompileShader(shader);
    
    CheckShaderError(shader, GL_COMPILE_STATUS, false, "Error: Shader compilation failed! ");
    
    return shader;
}


void Shader::Bind(){
    // bind to use the shader program
    glUseProgram(m_program);
}

void Shader::UnBind(){
    //unbind and stop using the shader program
    glUseProgram(0);
}

void Shader::SetTransfromUniform(const Transform & transform,
                         Camera * camera){
    
    //here we model the mvp, meaning the model, view, and projection matrices of the shader
    
    m_camera = camera;
    
    //getting the model matrix
    glm::mat4 model =  transform.GetModel();
    
    //getting the projection and view matrix
    glm::mat4 view = camera->GetViewMatrix();
    
    float aspect = (float)SCREEN_WIDTH/(float)SCREEN_HEIGHT;
    camera->SetPerspectiveProjectionMatrix(camera->GetZoom(),aspect, 0.1f, 1000.0f);
    
    glm::mat4 projection = camera->GetPerspectiveProjectionMatrix();
    
    glm::mat4 MVP = projection * view * model;
    
    //MVP = glm::mat4(1);//set to identity
    //MVP = glm::translate( MVP, glm::vec3( 0.5f, -0.5f, 0.0f ) );
    //MVP = glm::rotate( MVP, ( GLfloat)glfwGetTime( ) * -5.0f, glm::vec3( 0.0f, 0.0f, 1.0f ) );
    
    //openGL provides a function to update the uniform, this is called glUniform which has many types
    //the first parameter is the uniform we want to update
    //the next parameter is how many things we are sending in, which is one
    //the next parameter is to tell whether we are interested in transposing our matrix, you you might actualy just want to transpose it in the order openGL expects, however glm already has it in the proper order so we can pass in GL_FALSE, but if you are writing your own matrix class and for some reason your having issues even when you know your maths is right, then you migh just need to transpos it
    //the final variable is the actual data. here we are getting the address of the model matrix and accessing it as an array, which should provide the address of the first element in the model matrix
    glUniformMatrix4fv(m_uniforms[MVP_U], 1, GL_FALSE, &MVP[0][0]);
    
    glUniformMatrix4fv(m_uniforms[MODEL_U], 1, GL_FALSE, &model[0][0]);
    
    glUniformMatrix4fv(m_uniforms[VIEW_U], 1, GL_FALSE, &view[0][0]);
    
    glUniformMatrix4fv(m_uniforms[PROJECTION_U], 1, GL_FALSE, &projection[0][0]);

}

void Shader::SetMaterialUniform(const bool & bUseTexture){

    //one is true and zero is false
    glUniform1i(m_uniforms[USETEXTURE_U], bUseTexture);
    
    // light 
    glUniform3fv(m_uniforms[VIEWPOSITION_U], 1, glm::value_ptr(m_camera->GetPosition()));
    
    // Material
    glUniform1i( m_uniforms[SAMPLER_U],  0 );
    
    glUniform1i( m_uniforms[MATERIALNORMALSAMPLER_U],  1 );
    
    glUniform1i( m_uniforms[MATERIALDIFFUSESAMPLER_U], 2 );
    
    glUniform1i( m_uniforms[MATERIALSPECULARSAMPLER_U], 3 );
    
    glUniform1f(m_uniforms[MATERIALSHININESS_U], 32.0f);
    
    glUniform1f(m_uniforms[MATERIALINTENSITY_U], 9.0f);
    
    glUniform3fv(m_uniforms[MATERIALAMBIENT_U], 1, glm::value_ptr(glm::vec3(1.0f, 0.5f, 0.31f)));
    
    glUniform3fv(m_uniforms[MATERIALDIFFUSE_U], 1, glm::value_ptr(glm::vec3(1.0f, 0.5f, 0.31f)));
    
    glUniform3fv(m_uniforms[MATERIALSPECULAR_U], 1, glm::value_ptr(glm::vec3(0.5f, 0.5f, 0.5f)));
    
}

std::string Shader::LoadShader(const std::string& fileName)
{
    std::ifstream file;
    file.open((fileName).c_str());
    
    std::string output;
    std::string line;
    
    if(file.is_open())
    {
        while(file.good())
        {
            getline(file, line);
            output.append(line + "\n");
        }
    }
    else
    {
        std::cerr << "Unable to load shader: " << fileName << std::endl;
    }
    
    return output;
}

void Shader::CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage)
{
    GLint success = 0;
    GLchar error[1024] = { 0 };
    
    if(isProgram){
        glGetProgramiv(shader, flag, &success);
    }else{
        glGetShaderiv(shader, flag, &success);
    }
    
    if(success == GL_FALSE)
    {
        if(isProgram)
            glGetProgramInfoLog(shader, sizeof(error), NULL, error);
        else
            glGetShaderInfoLog(shader, sizeof(error), NULL, error);
        
        std::cerr << errorMessage << ": '" << error << "'" << std::endl;
    }
    
}

void Shader::CreateDirectionalLightUniform(const std::string & uniformName)
{
    m_uniforms[DIRECTIONALLIGHTCOLOR_U] = glGetUniformLocation(m_program, (uniformName + ".base.color").c_str());
    
    m_uniforms[DIRECTIONALLIGHTINTENSITY_U] = glGetUniformLocation(m_program, (uniformName + ".base.intensity").c_str());
    
    m_uniforms[DIRECTIONALLIGHTDIRECTION_U] = glGetUniformLocation(m_program, (uniformName + ".direction").c_str());
    
}

void Shader::CreatePointLightUniform(const std::string & uniformName)
{
    
    m_uniforms[POINTLIGHTCOLOR_U] = glGetUniformLocation(m_program, (uniformName + ".base.color").c_str());
    
    m_uniforms[POINTLIGHTINTENSITY_U] = glGetUniformLocation(m_program, (uniformName + ".base.intensity").c_str());
    
    m_uniforms[POINTLIGHTCONSTANT_U] = glGetUniformLocation(m_program, (uniformName + ".atten.constant").c_str());
    
    m_uniforms[POINTLIGHTLINEAR_U] = glGetUniformLocation(m_program, (uniformName + ".atten.linear").c_str());
    
    m_uniforms[POINTLIGHTEXPONENT_U] = glGetUniformLocation(m_program, (uniformName + ".atten.exponent").c_str());
    
    m_uniforms[POINTLIGHTPOSITION_U] = glGetUniformLocation(m_program, (uniformName + ".position").c_str());
    
    m_uniforms[POINTLIGHTRANGE_U] = glGetUniformLocation(m_program, (uniformName + ".range").c_str());
    
}

void Shader::CreateSpotLightUniform(const std::string& uniformName)
{
    m_uniforms[SPOTTLIGHTCOLOR_U] = glGetUniformLocation(m_program, (uniformName + ".pointLight.base.color").c_str());
    
    m_uniforms[SPOTLIGHTINTENSITY_U] = glGetUniformLocation(m_program, (uniformName + ".pointLight.base.intensity").c_str());
    
    m_uniforms[SPOTLIGHTCONSTANT_U] = glGetUniformLocation(m_program, (uniformName + ".pointLight.atten.constant").c_str());
    
    m_uniforms[SPOTLIGHTLINEAR_U] = glGetUniformLocation(m_program, (uniformName + ".pointLight.atten.linear").c_str());
    
    m_uniforms[SPOTLIGHTEXPONENT_U] = glGetUniformLocation(m_program, (uniformName + ".pointLight.atten.exponent").c_str());
    
    m_uniforms[SPOTLIGHTPOSITION_U] = glGetUniformLocation(m_program, (uniformName + ".pointLight.position").c_str());
    
    m_uniforms[SPOTLIGHTRANGE_U] = glGetUniformLocation(m_program, (uniformName + ".pointLight.range").c_str());
    
    m_uniforms[SPOTLIGHTDIRECTION_U] = glGetUniformLocation(m_program, (uniformName + ".direction").c_str());
    
    m_uniforms[SPOTLIGHTCUTOFF_U] = glGetUniformLocation(m_program, (uniformName + ".cutoff").c_str());
    
    m_uniforms[SPOTLIGHTOUTERCUTOFF_U] = glGetUniformLocation(m_program, (uniformName + ".outerCutoff").c_str());
    
}


void Shader::SetDirectionalLightUniform(const DirectionalLight& directionalLight)
{
    glUniform3fv(m_uniforms[DIRECTIONALLIGHTCOLOR_U], 1, glm::value_ptr(directionalLight.color));
    
    glUniform1f(m_uniforms[DIRECTIONALLIGHTINTENSITY_U], directionalLight.intensity);
    
    glUniform3f(m_uniforms[DIRECTIONALLIGHTDIRECTION_U], 3.2f, 3.0f, -0.3f); // for directional light
    //glUniform3fv(m_uniforms[DIRECTIONALLIGHTDIRECTION_U], 1, glm::value_ptr( m_mainlightPosition) ); 
}
 
void Shader::SetPointLightUniform(const PointLight& pointLight, const glm::vec3& position)
{

    glUniform3fv(m_uniforms[POINTLIGHTCOLOR_U], 1, glm::value_ptr(pointLight.color));
    
    glUniform1f(m_uniforms[POINTLIGHTINTENSITY_U], pointLight.intensity);
    
    glUniform3fv(m_uniforms[POINTLIGHTPOSITION_U], 1, glm::value_ptr( position) );
    
    glUniform1f(m_uniforms[POINTLIGHTCONSTANT_U], pointLight.atten.constant);
    
    glUniform1f(m_uniforms[POINTLIGHTLINEAR_U], pointLight.atten.linear);
    
    glUniform1f(m_uniforms[POINTLIGHTEXPONENT_U], pointLight.atten.exponent);
    
    glUniform1f(m_uniforms[POINTLIGHTRANGE_U], pointLight.range);
    
}


void Shader::SetSpotLightUniform(const SpotLight& spotLight)
{

    glUniform3fv(m_uniforms[SPOTTLIGHTCOLOR_U], 1, glm::value_ptr(spotLight.color));
    
    glUniform1f(m_uniforms[SPOTLIGHTINTENSITY_U], spotLight.intensity);
    
    glUniform1f(m_uniforms[SPOTLIGHTCONSTANT_U], spotLight.atten.constant);
    
    glUniform1f(m_uniforms[SPOTLIGHTLINEAR_U], spotLight.atten.linear);
    
    glUniform1f(m_uniforms[SPOTLIGHTEXPONENT_U], spotLight.atten.exponent);
    
    glUniform3fv(m_uniforms[SPOTLIGHTPOSITION_U], 1, glm::value_ptr( m_camera->GetPosition()) );
    
    glUniform1f(m_uniforms[SPOTLIGHTRANGE_U], spotLight.range);
    
    glUniform3fv(m_uniforms[SPOTLIGHTDIRECTION_U], 1, glm::value_ptr(m_camera->GetForward()) );
    
    glUniform1f(m_uniforms[SPOTLIGHTCUTOFF_U], spotLight.cutoff);
    
    glUniform1f(m_uniforms[SPOTLIGHTOUTERCUTOFF_U], spotLight.outerCutoff);
    
}
