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
    
    //glGetUniformLocation takes two parameters, 
    //first it gets the program we want to get the uniform from
    //then the second is the uniform variable we want to get, it is the name of the unform variable
    m_uniforms[MVP_U] = glGetUniformLocation(m_program, "MVP");
    
    m_uniforms[MODEL_U] = glGetUniformLocation(m_program, "model");
    
    m_uniforms[VIEW_U] = glGetUniformLocation(m_program, "view");
    
    m_uniforms[PROJECTION_U] = glGetUniformLocation(m_program, "projection");
    
    m_uniforms[USETEXTURE_U]  = glGetUniformLocation(m_program, "bUseTexture");
    
    m_uniforms[VIEWPOSITION_U] = glGetUniformLocation(m_program, "viewPosition");
    
    
    //Light 
    m_uniforms[LIGHTCOLOR_U] = glGetUniformLocation(m_program, "light.color");
    
    m_uniforms[LIGHTPOSITION_U] = glGetUniformLocation(m_program, "light.position");
    
    m_uniforms[LIGHTDIRECTION_U] = glGetUniformLocation(m_program, "light.direction");
    
    m_uniforms[LIGHTWORLDDIRECTION_U] = glGetUniformLocation(m_program, "light.worldDirection");
    
    m_uniforms[LIGHTCUTOFF_U] = glGetUniformLocation(m_program, "light.cutOff");
    
    m_uniforms[LIGHTOUTERCUTOFF_U] = glGetUniformLocation(m_program, "light.outerCutOff");
    
    m_uniforms[LIGHTAMBIENT_U] = glGetUniformLocation(m_program, "light.ambient");
    
    m_uniforms[LIGHTDIFFUSE_U] = glGetUniformLocation(m_program, "light.diffuse");
    
    m_uniforms[LIGHTSPECULAR_U] = glGetUniformLocation(m_program, "light.specular");
    
    m_uniforms[LIGHTCONSTANT_U] = glGetUniformLocation(m_program, "light.constant");
    
    m_uniforms[LIGHTLINEAR_U] = glGetUniformLocation(m_program, "light.linear");
    
    m_uniforms[LIGHTQUADRATIC_U] = glGetUniformLocation(m_program, "light.quadratic");
    
    
    //Material
    m_uniforms[SAMPLER_U] = glGetUniformLocation(m_program, "material.sampler");
    
    m_uniforms[NORMALSAMPLER_U] = glGetUniformLocation(m_program, "material.normalSampler");
    
    m_uniforms[DIFFUSESAMPLER_U] = glGetUniformLocation(m_program, "material.diffuseSampler");
    
    m_uniforms[SPECULARSAMPLER_U] = glGetUniformLocation(m_program, "material.specularSampler");
    
    m_uniforms[SHININESS_U] = glGetUniformLocation(m_program, "material.shininess");
    
    m_uniforms[INTENSITY_U] = glGetUniformLocation(m_program, "material.intensity");
    
}

Shader::~Shader(){
    
    for (unsigned int i = 0; i < NUM_SHADERS; i++){
        glDetachShader(m_program, m_shaders[i]);
        glDeleteShader(m_shaders[i]);
    }
    
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
    
    //std::cout << text << endl;
    
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

void Shader::Update(
                    const Transform & transform, 
                    Camera * camera, 
                    const bool & bUseTexture, 
                    const glm::vec3 &lighColor,
                    const glm::vec3 &lighPosition
                    ){
    //here we model the mvp, meaning the model, view, and projection matrices of the shader
    
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
    
    //one is true and zero is false
    glUniform1i(m_uniforms[USETEXTURE_U], bUseTexture);
    
    // light 
    glUniform3fv(m_uniforms[VIEWPOSITION_U], 1, glm::value_ptr(camera->GetPosition()));
    
    glUniform3fv(m_uniforms[LIGHTCOLOR_U], 1, glm::value_ptr(lighColor));
    
    glUniform3fv(m_uniforms[LIGHTPOSITION_U], 1, glm::value_ptr(lighPosition)); // for point light
    //glUniform3fv(m_uniforms[LIGHTPOSITION_U], 1, glm::value_ptr(camera->GetPosition())); //for spot light
    
    glUniform3f(m_uniforms[LIGHTWORLDDIRECTION_U], -0.2f, 1.0f, -0.3f); // for directional light
    
    glUniform3fv(m_uniforms[LIGHTDIRECTION_U], 1, glm::value_ptr(camera->GetForward()));
    
    glUniform1f(m_uniforms[LIGHTCUTOFF_U], glm::cos(glm::radians(12.5f)));
    
    glUniform1f(m_uniforms[LIGHTOUTERCUTOFF_U], glm::cos(glm::radians(17.5f)));
    
    glUniform3fv(m_uniforms[LIGHTAMBIENT_U], 1, glm::value_ptr(glm::vec3(0.2f, 0.2f, 0.2f)));
    
    glUniform3fv(m_uniforms[LIGHTDIFFUSE_U], 1, glm::value_ptr(glm::vec3(0.9f, 0.9f, 0.9f)));
    
    glUniform3fv(m_uniforms[LIGHTSPECULAR_U], 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
    
    glUniform1f(m_uniforms[LIGHTCONSTANT_U], 1.0f);
    
    glUniform1f(m_uniforms[LIGHTLINEAR_U], 0.09f);
    
    glUniform1f(m_uniforms[LIGHTQUADRATIC_U], 0.032f);
    
    
    // Material
    glUniform1i( m_uniforms[SAMPLER_U],  0 );
    
    glUniform1i( m_uniforms[NORMALSAMPLER_U],  1 );
    
    glUniform1i( m_uniforms[DIFFUSESAMPLER_U], 2 );
    
    glUniform1i( m_uniforms[SPECULARSAMPLER_U], 3 );
    
    glUniform1f(m_uniforms[SHININESS_U], 32.0f);
    
    glUniform1f(m_uniforms[INTENSITY_U], 20.0f);
    
    
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
