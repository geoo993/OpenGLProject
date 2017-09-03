//
//  Shader.cpp
//
//  Created by GEORGE QUENTIN on 25/02/2017.
//  Copyright © 2017 LEXI LABS. All rights reserved.
//

#include "Shader.h"

void Shader::Create(const std::string &fileName){
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

void Shader::Bind(){
    // bind to use the shader program
    glUseProgram(m_program);
}

void Shader::UnBind(){
    //unbind and stop using the shader program
    glUseProgram(0);
}

void Shader::CreateTransformUniform()
{
    //glGetUniformLocation takes two parameters,
    //first it gets the program we want to get the uniform from
    //then the second is the uniform variable we want to get, it is the name of the unform variable
    m_uniforms[MVP_U] = glGetUniformLocation(m_program, "MVP");
    
    m_uniforms[MODEL_U] = glGetUniformLocation(m_program, "model");
    
    m_uniforms[VIEW_U] = glGetUniformLocation(m_program, "view");
    
    m_uniforms[PROJECTION_U] = glGetUniformLocation(m_program, "projection");

}

void Shader::CreateMaterialUniform(const std::string & uniformName){
    
    //Material
    m_uniforms[SAMPLER_U] = glGetUniformLocation(m_program, (uniformName + ".sampler").c_str());
    
    m_uniforms[MATERIALNORMALSAMPLER_U] = glGetUniformLocation(m_program, (uniformName + ".normalSampler").c_str());
    
    m_uniforms[MATERIALDIFFUSESAMPLER_U] = glGetUniformLocation(m_program, (uniformName + ".diffuseSampler").c_str());
    
    m_uniforms[MATERIALSPECULARSAMPLER_U] = glGetUniformLocation(m_program, (uniformName + ".specularSampler").c_str());
    
    m_uniforms[MATERIALSHININESS_U] = glGetUniformLocation(m_program, (uniformName + ".shininess").c_str());
    
    m_uniforms[MATERIALINTENSITY_U] = glGetUniformLocation(m_program, (uniformName + ".intensity").c_str());
    
    m_uniforms[MATERIALAMBIENT_U] = glGetUniformLocation(m_program, (uniformName + ".ambient").c_str());
    
    m_uniforms[MATERIALDIFFUSE_U] = glGetUniformLocation(m_program, (uniformName + ".diffuse").c_str());
    
    m_uniforms[MATERIALSPECULAR_U] = glGetUniformLocation(m_program, (uniformName + ".specular").c_str());
    
}


void Shader::SetTransfromUniform(const Transform & transform,
                                 Camera * camera){
    
    CreateTransformUniform();
    
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


void Shader::SetDeclaredUniform(const bool & bUseTexture, const glm::vec3 & lightColor ){
    
    SetUniform("bUseTexture", bUseTexture);
    SetUniform("viewPosition", m_camera->GetPosition());
    SetUniform("lightColor", lightColor);
}



void Shader::SetMaterialUniform(){
    
    CreateMaterialUniform("material");
    
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

void Shader::SetDirectionalLightUniform(const std::string &uniformName, const DirectionalLight& directionalLight)
{
    SetUniform((uniformName + ".base.color").c_str(), directionalLight.color);
    SetUniform((uniformName + ".base.intensity").c_str(), directionalLight.intensity);
    SetUniform((uniformName + ".direction").c_str(), glm::vec3(3.2f, 3.0f, -0.3f));
    
}
 
void Shader::SetPointLightUniform(const std::string &uniformName, const PointLight& pointLight, const glm::vec3& position)
{
    // POINT LIGHT
    SetUniform((uniformName + ".base.color").c_str(), pointLight.color);
    SetUniform((uniformName + ".base.intensity").c_str(), pointLight.intensity);
    SetUniform((uniformName + ".atten.constant").c_str(), pointLight.atten.constant);
    SetUniform((uniformName + ".atten.linear").c_str(), pointLight.atten.linear);
    SetUniform((uniformName + ".atten.exponent").c_str(), pointLight.atten.exponent);
    SetUniform((uniformName + ".position").c_str(), position);
    SetUniform((uniformName + ".range").c_str(), pointLight.range);
    
}

void Shader::SetSpotLightUniform(const std::string &uniformName, const SpotLight& spotLight)
{
    // SPOT LIGHT

    SetUniform((uniformName + ".pointLight.base.color").c_str(), spotLight.color);
    SetUniform((uniformName + ".pointLight.base.intensity").c_str(), spotLight.intensity);
    SetUniform((uniformName + ".pointLight.atten.constant").c_str(), spotLight.atten.constant);
    SetUniform((uniformName + ".pointLight.atten.linear").c_str(), spotLight.atten.linear);
    SetUniform((uniformName + ".pointLight.atten.exponent").c_str(), spotLight.atten.exponent);
    SetUniform((uniformName + ".pointLight.position").c_str(), m_camera->GetPosition() );
    SetUniform((uniformName + ".pointLight.range").c_str(), spotLight.range);
    SetUniform((uniformName + ".direction").c_str(), m_camera->GetForward() );
    SetUniform((uniformName + ".cutoff").c_str(), spotLight.cutoff);
    SetUniform((uniformName + ".outerCutoff").c_str(), spotLight.outerCutoff);
    
}


// A collection of functions to set uniform variables inside shaders

// Setting floats

void Shader::SetUniform(std::string sName, float* fValues, int iCount)
{
    int iLoc = glGetUniformLocation(m_program, sName.c_str());
    glUniform1fv(iLoc, iCount, fValues);
}

void Shader::SetUniform(std::string sName, const float fValue)
{
    int iLoc = glGetUniformLocation(m_program, sName.c_str());
    glUniform1fv(iLoc, 1, &fValue);
}

// Setting vectors

void Shader::SetUniform(std::string sName, glm::vec2* vVectors, int iCount)
{
    int iLoc = glGetUniformLocation(m_program, sName.c_str());
    glUniform2fv(iLoc, iCount, (GLfloat*)vVectors);
    
}


void Shader::SetUniform(std::string sName, const glm::vec2 vVector)
{
    int iLoc = glGetUniformLocation(m_program, sName.c_str());
    glUniform2fv(iLoc, 1, (GLfloat*)&vVector);
}

void Shader::SetUniform(std::string sName, glm::vec3* vVectors, int iCount)
{
    int iLoc = glGetUniformLocation(m_program, sName.c_str());
    glUniform3fv(iLoc, iCount, (GLfloat*)vVectors);
}

void Shader::SetUniform(std::string sName, const glm::vec3 vVector)
{
    int iLoc = glGetUniformLocation(m_program, sName.c_str());
    glUniform3fv(iLoc, 1, (GLfloat*)&vVector);
}

void Shader::SetUniform(std::string sName, glm::vec4* vVectors, int iCount)
{
    int iLoc = glGetUniformLocation(m_program, sName.c_str());
    glUniform4fv(iLoc, iCount, (GLfloat*)vVectors);
}

void Shader::SetUniform(std::string sName, const glm::vec4 vVector)
{
    int iLoc = glGetUniformLocation(m_program, sName.c_str());
    glUniform4fv(iLoc, 1, (GLfloat*)&vVector);
}

// Setting 3x3 matrices

void Shader::SetUniform(std::string sName, glm::mat3* mMatrices, int iCount)
{
    int iLoc = glGetUniformLocation(m_program, sName.c_str());
    glUniformMatrix3fv(iLoc, iCount, false, (GLfloat*)mMatrices);
}

void Shader::SetUniform(std::string sName, const glm::mat3 mMatrix)
{
    int iLoc = glGetUniformLocation(m_program, sName.c_str());
    glUniformMatrix3fv(iLoc, 1, false, (GLfloat*)&mMatrix);
}

// Setting 4x4 matrices

void Shader::SetUniform(std::string sName, glm::mat4* mMatrices, int iCount)
{
    int iLoc = glGetUniformLocation(m_program, sName.c_str());
    glUniformMatrix4fv(iLoc, iCount, false, (GLfloat*)mMatrices);
}

void Shader::SetUniform(std::string sName, const glm::mat4 mMatrix)
{
    int iLoc = glGetUniformLocation(m_program, sName.c_str());
    glUniformMatrix4fv(iLoc, 1, false, (GLfloat*)&mMatrix);
}

// Setting integers

void Shader::SetUniform(std::string sName, int* iValues, int iCount)
{
    int iLoc = glGetUniformLocation(m_program, sName.c_str());
    glUniform1iv(iLoc, iCount, iValues);
}

void Shader::SetUniform(std::string sName, const int iValue)
{
    int iLoc = glGetUniformLocation(m_program, sName.c_str());
    glUniform1i(iLoc, iValue);
}
