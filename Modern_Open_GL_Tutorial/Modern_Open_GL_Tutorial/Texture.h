//
//  Texture.h
//
//  Created by GEORGE QUENTIN on 25/02/2017.
//  Copyright © 2017 LEXI LABS. All rights reserved.
//

#ifndef Texture_h
#define Texture_h

#include "Common.h"

class Texture {
    
public:
    
    Texture();
    void Create(const std::string &filepath, const bool &generateMipMaps = true);
    void CreateFromData(BYTE* imageData, int width, int height, int bpp, GLenum format, bool generateMipMaps = false);
    
    void SetSamplerObjectParameter(GLenum parameter, GLenum value);
    void SetSamplerObjectParameterf(GLenum parameter, float value);
    
    void Bind(unsigned int textureUnit = 0);
    void UnBind();
    
    int GetWidth();
    int GetHeight();
    int GetBPP();
 
    void Release();
    
    virtual ~Texture();
    Texture(const Texture &other){}
    void operator=(const Texture &other){}
    
private:
    
    int m_width, m_height, m_bpp; // Texture width, height, and bytes per pixel
    GLuint m_texture; // Texture id
    GLuint m_sampler; // Sampler id
    bool m_mipMapsGenerated;
    
    string m_path;
    
};

#endif /* Texture_h */
