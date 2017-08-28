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
    
    
    // DIRECTIONAL LIGHT
    m_uniforms[LIGHTCOLOR_U] = glGetUniformLocation(m_program, "directionallight.base.color");
    
    m_uniforms[LIGHTINTENSITY_U] = glGetUniformLocation(m_program, "directionallight.base.intensity");
    
    m_uniforms[DIRECTIONALLIGHTWORLDDIRECTION_U] = glGetUniformLocation(m_program, "directionallight.worldDirection");
    
    m_uniforms[DIRECTIONALLIGHTAMBIENT_U] = glGetUniformLocation(m_program, "directionallight.ambient");
    
    m_uniforms[DIRECTIONALLIGHTDIFFUSE_U] = glGetUniformLocation(m_program, "directionallight.diffuse");
    
    m_uniforms[DIRECTIONALLIGHTSPECULAR_U] = glGetUniformLocation(m_program, "directionallight.specular");
    
    
    // POINT LIGHT 
    m_uniforms[LIGHTCOLOR_U] = glGetUniformLocation(m_program, "pointlights[0].base.color");
    
    m_uniforms[LIGHTINTENSITY_U] = glGetUniformLocation(m_program, "pointlights[0].base.intensity");
    
    m_uniforms[POINTLIGHTPOSITION_A_U] = glGetUniformLocation(m_program, "pointlight[0].position");
    m_uniforms[POINTLIGHTPOSITION_B_U] = glGetUniformLocation(m_program, "pointlight[1].position");
    m_uniforms[POINTLIGHTPOSITION_C_U] = glGetUniformLocation(m_program, "pointlight[2].position");
    m_uniforms[POINTLIGHTPOSITION_D_U] = glGetUniformLocation(m_program, "pointlight[3].position");
    
    m_uniforms[POINTLIGHTAMBIENT_U] = glGetUniformLocation(m_program, "pointlights[0].ambient");
    
    m_uniforms[POINTLIGHTDIFFUSE_U] = glGetUniformLocation(m_program, "pointlights[0].diffuse");
    
    m_uniforms[POINTLIGHTSPECULAR_U] = glGetUniformLocation(m_program, "pointlights[0].specular");
    
    m_uniforms[POINTLIGHTCONSTANT_U] = glGetUniformLocation(m_program, "pointlights[0].constant");
    
    m_uniforms[POINTLIGHTLINEAR_U] = glGetUniformLocation(m_program, "pointlights[0].linear");
    
    m_uniforms[POINTLIGHTQUADRATIC_U] = glGetUniformLocation(m_program, "pointlights[0].quadratic");
    
    // SPOT LIGHT
    m_uniforms[SPOTLIGHTPOSITION_U] = glGetUniformLocation(m_program, "spotlight.position");
    
    m_uniforms[SPOTLIGHTDIRECTION_U] = glGetUniformLocation(m_program, "spotlight.direction");
    
    m_uniforms[POINTLIGHTAMBIENT_U] = glGetUniformLocation(m_program, "spotlight.ambient");
    
    m_uniforms[POINTLIGHTDIFFUSE_U] = glGetUniformLocation(m_program, "spotlight.diffuse");
    
    m_uniforms[POINTLIGHTSPECULAR_U] = glGetUniformLocation(m_program, "spotlight.specular");
    
    m_uniforms[POINTLIGHTCONSTANT_U] = glGetUniformLocation(m_program, "spotlight.constant");
    
    m_uniforms[POINTLIGHTLINEAR_U] = glGetUniformLocation(m_program, "spotlight.linear");
    
    m_uniforms[POINTLIGHTQUADRATIC_U] = glGetUniformLocation(m_program, "spotlight.quadratic");
    
    m_uniforms[SPOTLIGHTCUTOFF_U] = glGetUniformLocation(m_program, "spotlight.cutOff");
    
    m_uniforms[SPOTLIGHTOUTERCUTOFF_U] = glGetUniformLocation(m_program, "spotlight.outerCutOff");
    
    
    //Material
    m_uniforms[SAMPLER_U] = glGetUniformLocation(m_program, "material.sampler");
    
    m_uniforms[MATERIALNORMALSAMPLER_U] = glGetUniformLocation(m_program, "material.normalSampler");
    
    m_uniforms[MATERIALDIFFUSESAMPLER_U] = glGetUniformLocation(m_program, "material.diffuseSampler");
    
    m_uniforms[MATERIALSPECULARSAMPLER_U] = glGetUniformLocation(m_program, "material.specularSampler");
    
    m_uniforms[MATERIALSHININESS_U] = glGetUniformLocation(m_program, "material.shininess");
    
    m_uniforms[MATERIALAMBIENT_U] = glGetUniformLocation(m_program, "material.ambient");
    
    m_uniforms[MATERIALDIFFUSE_U] = glGetUniformLocation(m_program, "material.diffuse");
    
    m_uniforms[MATERIALSPECULAR_U] = glGetUniformLocation(m_program, "material.specular");
    
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
    
    glUniform1f(m_uniforms[LIGHTINTENSITY_U], 20.0f);
    
    // Directional light
    glUniform3f(m_uniforms[DIRECTIONALLIGHTWORLDDIRECTION_U], -0.2f, -1.0f, -0.3f); // for directional light
    
    glUniform3fv(m_uniforms[DIRECTIONALLIGHTAMBIENT_U], 1, glm::value_ptr(glm::vec3(0.05f, 0.05f, 0.05f)));
    
    glUniform3fv(m_uniforms[DIRECTIONALLIGHTDIFFUSE_U], 1, glm::value_ptr(glm::vec3(0.4f, 0.4f, 0.4f)));
    
    glUniform3fv(m_uniforms[DIRECTIONALLIGHTSPECULAR_U], 1, glm::value_ptr(glm::vec3(0.6f, 0.6f, 0.6f)));
    
    // Point Light
    // Positions of the point lights
    glm::vec3 pointLightPositions[] = {
        glm::vec3(  0.7f,  0.2f,  2.0f      ),
        glm::vec3(  2.3f, -3.3f, -4.0f      ),
        glm::vec3(  -4.0f,  2.0f, -12.0f    ),
        glm::vec3(  0.0f,  0.0f, -3.0f      )
    };
    
    ////glUniform3fv(m_uniforms[POINTLIGHTPOSITION_U], 1, glm::value_ptr(lighPosition)); // for point light
    glUniform3fv(m_uniforms[POINTLIGHTPOSITION_A_U], 1, glm::value_ptr(pointLightPositions[0]));
    
    glUniform3fv(m_uniforms[POINTLIGHTPOSITION_B_U], 1, glm::value_ptr(pointLightPositions[1]));
    
    glUniform3fv(m_uniforms[POINTLIGHTPOSITION_C_U], 1, glm::value_ptr(pointLightPositions[2]));
    
    glUniform3fv(m_uniforms[POINTLIGHTPOSITION_D_U], 1, glm::value_ptr(pointLightPositions[3]));
    
    glUniform3fv(m_uniforms[POINTLIGHTAMBIENT_U], 1, glm::value_ptr(glm::vec3(0.2f, 0.2f, 0.2f)));
    
    glUniform3fv(m_uniforms[POINTLIGHTDIFFUSE_U], 1, glm::value_ptr(glm::vec3(0.9f, 0.9f, 0.9f)));
    
    glUniform3fv(m_uniforms[POINTLIGHTSPECULAR_U], 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
    
    glUniform1f(m_uniforms[POINTLIGHTCONSTANT_U], 1.0f);
    
    glUniform1f(m_uniforms[POINTLIGHTLINEAR_U], 0.09f);
    
    glUniform1f(m_uniforms[POINTLIGHTQUADRATIC_U], 0.032f);
    
    // Spot Light
    glUniform3fv(m_uniforms[SPOTLIGHTPOSITION_U], 1, glm::value_ptr(camera->GetPosition())); //for spot light
    
    glUniform3fv(m_uniforms[SPOTLIGHTDIRECTION_U], 1, glm::value_ptr(camera->GetForward()));
    
    glUniform1f(m_uniforms[SPOTLIGHTCUTOFF_U], glm::cos(glm::radians(12.5f)));
    
    glUniform1f(m_uniforms[SPOTLIGHTOUTERCUTOFF_U], glm::cos(glm::radians(17.5f)));
    
    
    // Material
    glUniform1i( m_uniforms[SAMPLER_U],  0 );
    
    glUniform1i( m_uniforms[MATERIALNORMALSAMPLER_U],  1 );
    
    glUniform1i( m_uniforms[MATERIALDIFFUSESAMPLER_U], 2 );
    
    glUniform1i( m_uniforms[MATERIALSPECULARSAMPLER_U], 3 );
    
    glUniform1f(m_uniforms[MATERIALSHININESS_U], 32.0f);
    
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
