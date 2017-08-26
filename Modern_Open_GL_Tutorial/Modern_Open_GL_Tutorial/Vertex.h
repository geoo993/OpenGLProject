//
//  Vertex.h
//  ModernOpenGL_Basic
//
//  Created by GEORGE QUENTIN on 27/02/2017.
//  Copyright © 2017 LEXI LABS. All rights reserved.
//

#ifndef Vertex_h
#define Vertex_h

#include "Common.h"

class Vertex{
    
public:
    Vertex(const glm::vec3 &position, const glm::vec2 &texCoord, const glm::vec3 &normal = glm::vec3(0,0,0)){
        this->position = position;
        this->texCoord = texCoord;
        this->normal = normal;
    }
    
    inline glm::vec3* GetPositions() { return &position; }
    inline glm::vec2* GetTexCoordinates() { return &texCoord; }
    inline glm::vec3* GetNormals() { return &normal; }
    
    Vertex& operator=(const Vertex& other) // copy assignment
    {
        if (this != &other) { // self-assignment check expected
            this->position = other.position;
            this->texCoord = other.texCoord;
            this->normal = other.normal;
        }
        return *this;
    }
    
    Vertex(const Vertex & other){
        
        if (this != &other) { 
            this->position = other.position;
            this->texCoord = other.texCoord;
            this->normal = other.normal;
        }
    }
    
private:
    glm::vec3 position;
    glm::vec2 texCoord;
    glm::vec3 normal;
    
};


#endif /* Vertex_h */
