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
        
        this->m_position = position;
        this->m_projectionMatrix = glm::perspective(fieldOfView, aspect, zNear, zFar);
        this->m_forward = glm::vec3(0.0f, 0.0f, 1.0f);
        this->m_up = glm::vec3(0.0f, 1.0f, 0.0f);
        this->m_view = position + m_forward;
        
    }
    
    inline glm::mat4 GetViewProjection() const { 
        
        //this matrix is where you want the to look at 
        glm::mat4 cameraView = glm::lookAt(
                                           m_position, // what position you want the camera to be at when looking at something in World Space
                                           m_view, // // what position you want the camera to be  looking at in World Space, meaning look at what(using vec3) ?  // meaning the camera view point
                                           m_up  //which direction is up, you can set to (0,-1,0) to look upside-down
                                           );
        
        
        return m_projectionMatrix * cameraView;
    }
    
    inline glm::vec3 GetPosition() const { return m_position; }
    inline glm::vec3 GetView() const { return m_view; }
    inline glm::vec3 GetForward() const { return m_forward; }
    inline glm::vec3 GetUp() const { return m_up; }

    inline void SetPosition(const glm::vec3 &pos){  m_position = m_position + pos;  } 
    inline void SetView(const glm::vec3 &pos){  m_view = m_view + pos + m_forward; }
    
    virtual ~Camera(){}
private:
    Camera(const Camera &other){}
    void operator=(const Camera &other){}
    
    glm::mat4 m_projectionMatrix;
    glm::vec3 m_position;
    glm::vec3 m_view;
    glm::vec3 m_forward;
    glm::vec3 m_up;
    
    
};

#endif /* Camera_h */
