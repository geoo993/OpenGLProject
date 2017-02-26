//
//  Transform.hpp
//  ModernOpenGL_Basic
//
//  Created by GEORGE QUENTIN on 26/02/2017.
//  Copyright © 2017 LEXI LABS. All rights reserved.
//

#ifndef Transform_h
#define Transform_h

#include "Common.h"

class Transform{
    
public:
    Transform(const glm::vec3 &position = glm::vec3(), 
              const glm::vec3 &rotation = glm::vec3(), 
              const glm::vec3 &scale = glm::vec3(1.0f, 1.0f, 1.0f)   
              ): m_position(position), m_rotation(rotation), m_scale(scale) {}
    
    inline glm::mat4 GetModel() const { 
        
        //this generates a 4x4 matrix with a position vector
        glm::mat4 positionMatrix = glm::translate(m_position);
        
        //this generates a 4x4 matrix with a rotation vector, but these takes some angle
        //these are rotations to represent the axis and we use vec3(1,0,0) on x for suggesting the x axis
        glm::mat4 rotationXmatrix = glm::rotate(m_rotation.x, glm::vec3(1,0,0));
        glm::mat4 rotationYmatrix = glm::rotate(m_rotation.y, glm::vec3(0,1,0));
        glm::mat4 rotationZmatrix = glm::rotate(m_rotation.z, glm::vec3(0,0,1));
        
        //combining the rotation matrices into one rotion matrix, because of te way matrices are reepresented, you must write the multiplication in reverse order.
        glm::mat4 rotationMatrix = rotationZmatrix * rotationYmatrix * rotationXmatrix;
        
        //this generates a 4x4 matrix with a scale vector
        glm::mat4 scaleMatrix = glm::scale(m_scale);
        
        //must becareful with the order of multiplication, because you can get different result doing it differently
        return positionMatrix * rotationMatrix * scaleMatrix;
    }
    
    inline glm::vec3 & GetPositions() { return m_position; }
    inline glm::vec3 & GetRotation() { return m_rotation; }
    inline glm::vec3 & GetScale() { return m_scale; }
    
    inline void SetPositions(glm::vec3 & position) { m_position = position; }
    inline void SetRotation(glm::vec3 & rotation) { m_rotation = rotation; }
    inline void SetScale(glm::vec3 & scale) { m_scale = scale; }
    
    virtual ~Transform();
private:
    Transform(const Transform &other){}
    void operator=(const Transform &other){}
    
    glm::vec3 m_position;
    glm::vec3 m_rotation;
    glm::vec3 m_scale;
    
    glm::mat4 m_model;
    
};


#endif /* Transform_h */
