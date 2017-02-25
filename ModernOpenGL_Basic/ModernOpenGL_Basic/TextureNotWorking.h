//
//  Texture.hpp
//  ModernOpenGL_Basic
//
//  Created by GEORGE QUENTIN on 25/02/2017.
//  Copyright © 2017 LEXI LABS. All rights reserved.
//

#ifndef Texture_h
#define Texture_h

#include "Common.h"

class Texture {
    
public:
    Texture(const std::string &fileName);
    virtual ~Texture();
    
    void Bind(unsigned int textureUnit = 0);
    void UnBind();
    
    void Release();
    
private:
    Texture(const Texture &other){}
    void operator=(const Texture &other){}
    
    GLuint m_texture; // Texture id

};

#endif /* Texture_h */
