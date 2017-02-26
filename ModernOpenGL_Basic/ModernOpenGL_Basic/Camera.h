//
//  Camera.h
//  ModernOpenGL_Basic
//
//  Created by GEORGE QUENTIN on 26/02/2017.
//  Copyright © 2017 LEXI LABS. All rights reserved.
//

#ifndef Camera_h
#define Camera_h

#include "Common.h"

//Camera setup
class Camera{
    
public:
    
    Camera(const glm::vec3 &position, 
           const float &fieldOfView, 
           const float &aspect,
           const float &zNear, 
           const float &zFar) {
        
        m_perspectiveMatrix = glm::perspective(fieldOfView, aspect, zNear, zFar);
        m_position = position;
        m_forward = glm::vec3(0.0f, 0.0f, 1.0f);
        m_up = glm::vec3(0.0f, 1.0f, 0.0f);
        
    }
    
    inline glm::mat4 GetViewProjection() const { 
        
        //this matrix is where you want the to look at 
        glm::mat4 cameraView = glm::lookAt(
                                           m_position, // what position you want the camera to be at when looking at something in World Space
                                           m_position + m_forward, // // what position you want the camera to be  looking at in World Space, meaning look at what(using vec3) ?
                                           m_up  //which direction is up, you can set to (0,-1,0) to look upside-down
                                           );
        
        
        return m_perspectiveMatrix * cameraView;
    }
    
    inline glm::vec3 GetPosition() const { return m_position; }
    inline glm::vec3 GetForward() const { return m_forward; }
    inline glm::vec3 GetUp() const { return m_up; }
    
    virtual ~Camera(){}
private:
    Camera(const Camera &other){}
    void operator=(const Camera &other){}
    
    glm::mat4 m_perspectiveMatrix;
    glm::vec3 m_position;
    glm::vec3 m_forward;
    glm::vec3 m_up;
    
    
};

#endif /* Camera_h */
