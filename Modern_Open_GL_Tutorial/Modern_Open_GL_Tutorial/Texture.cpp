//
//  Texture.cpp
//
//  Created by GEORGE QUENTIN on 25/02/2017.
//  Copyright © 2017 LEXI LABS. All rights reserved.
//

#include "Texture.h"

Texture::Texture(){
    
}

GLuint Texture::CreateNewTexture(const std::string &filepath, const bool &generateMipMaps, GLint textureUnitAt) {
    
    //    Bitmap img(filePath.c_str());
    //    ////*-----------------------------------------------------------------------------
    //    ////*  Make some rgba data (can also load a file here)
    //    ////*-----------------------------------------------------------------------------
    
    //    int tw = img.width; 
    //    int th = img.height;
    FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
    FIBITMAP* dib(0);
    
    fif = FreeImage_GetFileType(filepath.c_str(), 0); // Check the file signature and deduce its format
    
    if(fif == FIF_UNKNOWN) // If still unknown, try to guess the file format from the file extension
        fif = FreeImage_GetFIFFromFilename(filepath.c_str());
    
    if(fif == FIF_UNKNOWN) // If still unknown, return failure
        return -1;
    
    if(FreeImage_FIFSupportsReading(fif)) // Check if the plugin has reading capabilities and load the file
        dib = FreeImage_Load(fif, filepath.c_str());
    
    if(!dib) {
        char message[1024];
        sprintf(message, "Cannot load image\n%s\n",filepath.c_str());
        return -1;
    }
    
    BYTE* imageData = FreeImage_GetBits(dib); // Retrieve the image data
    
    // If somehow one of these failed (they shouldn't), return failure
    if (imageData == nullptr || FreeImage_GetWidth(dib) == 0 || FreeImage_GetHeight(dib) == 0){
        std::cout << "Texture Loading failed: for texture " << filepath << std::endl;
        return -1;
    }
    
    GLint width = FreeImage_GetWidth(dib); 
    GLint height = FreeImage_GetHeight(dib);
    GLint bpp = FreeImage_GetBPP(dib);// bytes per pixel
    
    GLenum format;
    if(FreeImage_GetBPP(dib) == 32)format = GL_BGRA;
    if(FreeImage_GetBPP(dib) == 24)format = GL_BGR;
    if(FreeImage_GetBPP(dib) == 8)format = GL_LUMINANCE;
    
    // Generate an OpenGL texture ID for this texture
    //GLuint texture;
    m_texture = textureUnitAt;
    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    
    // set wrap mode
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    
    //float color[] = { 1.0f, 0.0f, 0.0f, 1.0f };
    //glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);
    
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    
    
    if(format == GL_RGBA || format == GL_BGRA)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, format, GL_UNSIGNED_BYTE, imageData);
    // We must handle this because of internal format parameter
    else if(format == GL_RGB || format == GL_BGR)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, imageData);
    else
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, imageData);
    
    
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, format,GL_UNSIGNED_BYTE, imageData);
    
    if(generateMipMaps)glGenerateMipmap(GL_TEXTURE_2D);
    
    if(generateMipMaps)glGenerateMipmap(GL_TEXTURE_2D);
    m_sampler = textureUnitAt;
    glGenSamplers(1, &m_sampler);
    
    m_path = "";
    m_mipMapsGenerated = generateMipMaps;
    m_width = width;
    m_height = height;
    m_bpp = bpp;
    
    
    return m_texture;
}



void Texture::Create(const std::string &filepath, const bool &generateMipMaps){
    
    
    //    Bitmap img(filePath.c_str());
    //    ////*-----------------------------------------------------------------------------
    //    ////*  Make some rgba data (can also load a file here)
    //    ////*-----------------------------------------------------------------------------
    
    //    int tw = img.width; 
    //    int th = img.height;
    FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
    FIBITMAP* dib(0);
    
    fif = FreeImage_GetFileType(filepath.c_str(), 0); // Check the file signature and deduce its format
    
    if(fif == FIF_UNKNOWN) // If still unknown, try to guess the file format from the file extension
        fif = FreeImage_GetFIFFromFilename(filepath.c_str());
    
    if(fif == FIF_UNKNOWN) // If still unknown, return failure
        return;
    
    if(FreeImage_FIFSupportsReading(fif)) // Check if the plugin has reading capabilities and load the file
        dib = FreeImage_Load(fif, filepath.c_str());
    
    if(!dib) {
        char message[1024];
        sprintf(message, "Cannot load image\n%s\n",filepath.c_str());
        return;
    }
    
    BYTE* imageData = FreeImage_GetBits(dib); // Retrieve the image data
    
    // If somehow one of these failed (they shouldn't), return failure
    if (imageData == nullptr || FreeImage_GetWidth(dib) == 0 || FreeImage_GetHeight(dib) == 0){
        std::cout << "Texture Loading failed: for texture " << filepath << std::endl;
        return;
    }
    
    GLenum format;
    int bada = FreeImage_GetBPP(dib);
    if(FreeImage_GetBPP(dib) == 32)format = GL_BGRA;
    if(FreeImage_GetBPP(dib) == 24)format = GL_BGR;
    if(FreeImage_GetBPP(dib) == 8)format = GL_LUMINANCE;
    CreateFromData(imageData, FreeImage_GetWidth(dib), FreeImage_GetHeight(dib), FreeImage_GetBPP(dib), format, generateMipMaps);
    
    FreeImage_Unload(dib);
    
    m_path = filepath;
    
    
}
 
// Create a texture from the data stored in bData.  
void Texture::CreateFromData(BYTE* imageData, int width, int height, int bpp, GLenum format, bool generateMipMaps)
{
   
    //std::cout << "width: " << width << ", height: " << height << std::endl;
    
    // generate some space for the texture, generate space for one texture and give us the space for that
    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    
    //before you can specify the texture, you can actually define other things and they are the following
    // here we can specify the wrap mode, just like bindtxture, the texparameteri is used to pass in the type of texture you want to create
    // you can pass in few different constants and specify them, like wrap mode for example
    //these settings controls texture wrapping and S is fixing texturing that go outside of the Witdh and T fixes the Height, GL_REPEAT tells you what behaviour should happen
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    
    
    // Nice trilinear filtering. this allows to handle interpolating or exterpolating through the texture filtering, and help in how it renders.
    //min filtering is when the texture take fewer pixels than its resolution, so min filter is for minification
    //GL_LINEAR means it will linearly interpolate the existing pixels and try to produce the most accurate image possible
    //Mag filtering is the exact opposite of Min filtering, so when a texture takes more pixels, it will try to linear interpolate so the texture is reproduced accurately
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    //glGenerateMipmap(GL_TEXTURE_2D);
    
    //handle formats
    if(format == GL_RGBA || format == GL_BGRA)
    {
        ///*-----------------------------------------------------------------------------
        ////*  Allocate Memory on the GPU
        //// *-----------------------------------------------------------------------------
        
        //Now we give the image to OpenGL
        // target | level lod | internal_format | width | height | border | format | type | data
        //GL_TEXTURE_2D specifies how openGL interprets the data being sent, telling it that it is a texture
        //the level helps to reduce filtering type effects, it is another technique to do minification and magnification. help to get the texture apprearing as crisp as it can. zero is the default level.
        //internal_format is how openGL is going to store the indivual pixels in the gpu, and tell it how to format them, RGBA means it is going to store the red, green, blue and alpha parts of the data
        //width and height are the size of the texture
        //border is set to zero by default, this is to add borders to your texture, meaning border lines
        //the format is the type of format of the data you are sending to the gpu, and the format here is an RGBA format
        //as for the type, this is actually the data type that you are sending in, meaning that you are telling openGL how the data is sorted, which in this case we are using BYTE
        //finally the pixels, this is the pointer that has all the data of the pixels
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, format, GL_UNSIGNED_BYTE, imageData);
            
    }
    // We must handle this because of internal format parameter
    else if(format == GL_RGB || format == GL_BGR){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, imageData);
        
    }else if(format == GL_BGRA || format == GL_BGRA){
        //    ////*-----------------------------------------------------------------------------
        //    //// *  Load data onto GPU (bitmaps flip RGB order)
        //    //// *-----------------------------------------------------------------------------
        //    // target | lod | xoffset | yoffset | width | height | format | type | data
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, format, GL_UNSIGNED_BYTE, imageData);
    
    }else{
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, imageData);
    }
    
    //glGenerateMipmap(GL_TEXTURE_2D);
    
    
    //openGL does all texture location by coordinates from zero to one (0 -> 1), zero being left of the texture and one to the right on x axis and zero being the bottom of the texture and one being the top on y axis. that way it is resolution inderpendent. so this means you can specify a texture of any resolution.
    //so a uniform sampler is the type of variable that is updated by the gpu which you can use to specify texture coordinates, this is a variable that both the gpu and the cpu have access to. a sampler in openGL is the type of data that a texture has or takes, or which you use to pass in texture data. the gpu reads this in the fragment shader
    
    
    if(generateMipMaps)glGenerateMipmap(GL_TEXTURE_2D);
    glGenSamplers(1, &m_sampler);
    
    m_path = "";
    m_mipMapsGenerated = generateMipMaps;
    m_width = width;
    m_height = height;
    m_bpp = bpp;
}

void Texture::SetSamplerObjectParameter(GLenum parameter, GLenum value)
{
    glSamplerParameteri(m_sampler, parameter, value);
}

void Texture::SetSamplerObjectParameterf(GLenum parameter, float value)
{
    glSamplerParameterf(m_sampler, parameter, value);
}

// Binds a texture for rendering
void Texture::Bind(unsigned int textureUnit)
{
    assert(textureUnit >= 0 && textureUnit <= 31);
    
    //glActiveTexture specifies which texture openGL is working with
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    
    glBindTexture(GL_TEXTURE_2D, m_texture);

    glBindSampler(textureUnit, m_sampler);
    
}

void Texture::UnBind()
{
    
    /////*-----------------------------------------------------------------------------
    //// *  Unbind texture
    ////*-----------------------------------------------------------------------------
    glBindTexture(GL_TEXTURE_2D, 0);
    
}

// Frees memory on the GPU of the texture
void Texture::Release()
{
    glDeleteSamplers(1, &m_sampler);
    glDeleteTextures(1, &m_texture);
}

int Texture::GetWidth()
{
    return m_width;
}

int Texture::GetHeight()
{
    return m_height;
}

int Texture::GetBPP()
{
    return m_bpp;
}

Texture::~Texture(){
    Release();
}

