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


// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum CameraMovement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Default camera values
const GLfloat YAW        = -90.0f;
const GLfloat PITCH      =  0.1f;
const GLfloat ROLL      =  0.1f;
const GLfloat SPEED      =  10.0f;
const GLfloat SENSITIVTY =  0.25f;
const GLfloat ZOOM       =  45.0f;


//Camera setup
class Camera{
    
public:
    
    Camera(){};
    virtual ~Camera(){}
    
    void Create( 
                const glm::vec3 &position = glm::vec3( 0.0f, 0.0f, -15.0f ),
                const glm::vec3 &worldUp = glm::vec3( 0.0f, 1.0f, 0.0f ),
                const GLfloat &pitch = PITCH, 
                const GLfloat &yaw = YAW, 
                const GLfloat &roll = ROLL, 
                const GLfloat &fieldOfView = 45.0f, 
                const GLfloat &width = (float)SCREEN_WIDTH,
                const GLfloat &height = (float)SCREEN_HEIGHT, 
                const GLfloat &zNear = 0.1f, 
                const GLfloat &zFar = 1000.0f) {
        
        this->m_position = position;
        this->m_worldUp = worldUp;
        this->m_fieldOfView = fieldOfView;
        this->SetPerspectiveProjectionMatrix(fieldOfView, (width/height), zNear, zFar);
        this->SetOrthographicProjectionMatrix(width,height,zNear, zFar );
        this->m_pitch = pitch;
        this->m_yaw = yaw;
        this->m_roll = roll;
        this->m_movementSpeed =  SPEED; 
        this->m_mouseSensitivity = SENSITIVTY;
        this->m_zoom = ZOOM;
        this->UpdateCameraVectors();
    }
    
    void Create( 
                const glm::vec3 &position,
                const glm::vec3 &worldUp,
                const GLfloat &fieldOfView, 
                const GLfloat &width,
                const GLfloat &height, 
                const GLfloat &zNear, 
                const GLfloat &zFar) {
        
        this->m_position = position;
        this->m_worldUp = worldUp;
        this->m_fieldOfView = fieldOfView;
        
        this->SetPerspectiveProjectionMatrix(fieldOfView, (width/height), zNear, zFar);
        this->SetOrthographicProjectionMatrix(width,height,zNear, zFar );
        this->m_pitch = PITCH;
        this->m_yaw = YAW;
        this->m_roll = ROLL;
        this->m_movementSpeed =  SPEED; 
        this->m_mouseSensitivity = SENSITIVTY;
        this->m_zoom = ZOOM;
        this->UpdateCameraVectors( );
    }
    
    void SetPerspectiveProjectionMatrix(const GLfloat &fieldOfView, const GLfloat &aspect, const GLfloat &zNear = 0.1f, const GLfloat &zFar = 5000.0f){
        this->m_fieldOfView = fieldOfView;
        this->m_perspectiveProjectionMatrix = glm::perspective(fieldOfView, aspect, zNear, zFar);
    }
    void SetOrthographicProjectionMatrix(const GLfloat &width, const GLfloat height, const GLfloat &zNear = 0.1f, const GLfloat &zFar = 5000.0f){
        this->m_orthographicProjectionMatrix = glm::ortho(0.0f, width, 0.0f, height, zNear, zFar);
    }
    
    inline glm::mat4 GetViewProjection() const { return this->m_perspectiveProjectionMatrix * this->m_viewMatrix; }
    inline glm::mat4 GetPerspectiveProjectionMatrix() const { return this->m_perspectiveProjectionMatrix; }
    inline glm::mat4 GetOrthographicProjectionMatrix() const { 
        return this-> m_orthographicProjectionMatrix; }	
    inline glm::mat4 GetViewMatrix() const { return this->m_viewMatrix; }
    
    inline glm::vec3 GetPosition() const { return this->m_position; }
    inline glm::vec3 GetView() const { return this->m_view; }
    inline glm::vec3 GetForward() const { return this->m_front; }
    inline glm::vec3 GetUp() const { return this->m_up; }
    inline glm::vec3 GetRight() const { return this->m_right; }
    inline GLfloat GetZoom( ){ return this->m_zoom; }
    
    // Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard( const CameraMovement &direction, const GLfloat &deltaTime )
    {
        GLfloat velocity = this->m_movementSpeed * deltaTime;
        
        if ( direction == FORWARD )
        {
            this->m_position += this->m_front * velocity;
        }
        
        if ( direction == BACKWARD )
        {
            this->m_position -= this->m_front * velocity;
        }
        
        if ( direction == LEFT )
        {
            this->m_position -= this->m_right * velocity;
        }
        
        if ( direction == RIGHT )
        {
            this->m_position += this->m_right * velocity;
        }
    }
    
    // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement( GLfloat &xOffset, GLfloat &yOffset, GLboolean constrainPitch = true )
    {
        xOffset *= this->m_mouseSensitivity;
        yOffset *= this->m_mouseSensitivity;
        
        this->m_pitch += yOffset; // up down
        this->m_yaw   += xOffset; //left right
        //this->m_roll += yOffset; // up down
        
        // Make sure that when pitch is out of bounds, screen doesn't get flipped
        if ( constrainPitch )
        {
            if ( this->m_pitch >= 90.0f )
            {
                this->m_pitch = -90.0f;
                this->m_roll = 0;
            }
            
            if ( this->m_pitch <= -90.0f )
            {
                this->m_pitch = -90.0f;
            }
          
        }
        
        // Update Front, Right and Up Vectors using the updated Eular angles
        this->UpdateCameraVectors( );
    }
    
    
private:
    Camera(const Camera &other){}
    void operator=(const Camera &other){}
    
    //view and projection matrix
    glm::mat4 m_perspectiveProjectionMatrix;
    glm::mat4 m_orthographicProjectionMatrix;
    glm::mat4 m_viewMatrix;
    
    // Camera Attributes
    glm::vec3 m_view;
    glm::vec3 m_position;
    glm::vec3 m_front;
    glm::vec3 m_back;
    glm::vec3 m_left;
    glm::vec3 m_right;
    glm::vec3 m_up;
    glm::vec3 m_down;
    glm::vec3 m_worldUp;
    
    // Eular Angles
    GLfloat m_fieldOfView;
    GLfloat m_yaw;
    GLfloat m_pitch;
    GLfloat m_roll;
    
    // Camera options
    GLfloat m_movementSpeed;
    GLfloat m_mouseSensitivity;
    GLfloat m_zoom;
    
    // Calculates the front vector from the Camera's (updated) Eular Angles
    void UpdateCameraVectors( )
    {
        // Calculate the new Front vector
        glm::vec3 front;
        front.x = cos( glm::radians( this->m_yaw ) ) * cos( glm::radians( this->m_pitch ) );
        front.y = sin( glm::radians( this->m_pitch ) );
        front.z = sin( glm::radians( this->m_yaw ) ) * cos( glm::radians( this->m_pitch ) );
        this->m_front = glm::normalize( front );
        //this->m_front = glm::vec3(0.0f, 0.0f, 1.0f);
       
        // Also re-calculate the Right and Up vector
        this->m_right = glm::normalize( glm::cross( this->m_front, this->m_worldUp ) );  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        this->m_up = glm::normalize( glm::cross( this->m_right, this->m_front ) );
        //this->m_up = glm::vec3(0.0f, 1.0f, 0.0f);
        
        this->m_view = m_position + m_front;
        this->m_viewMatrix = glm::lookAt(
                                         m_position, // what position you want the camera to be at when looking at something in World Space
                                         m_view, // // what position you want the camera to be  looking at in World Space, meaning look at what(using vec3) ?  // meaning the camera view point
                                         m_up  //which direction is up, you can set to (0,-1,0) to look upside-down
                                         );
      
    }
};

#endif /* Camera_h */
