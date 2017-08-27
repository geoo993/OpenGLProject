//
//  Game.hpp
//  Modern_Open_GL_Tutorial
//
//  Created by GEORGE QUENTIN on 26/08/2017.
//  Copyright © 2017 geomakesgames. All rights reserved.
//

#ifndef Game_h
#define Game_h

#include "Common.h"
#include "Vertex.h"
#include "Camera.h"
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"

class Game {
private:
    
    GLFWwindow* m_window;
    Camera *m_camera;
    
    void Initialise();
    void LoadFromResources(const std::string &resourcepath);
    void Render();
    void Update();
    void GameLoop();
    void GameTimer();
    
    bool m_appActive = false;
    GLfloat m_deltaTime, m_elapsedTime = 0.0f;
    std::string m_resources_path;
    
    Shader m_basicshader;
    Shader m_screenshader;
    Shader m_lightingshader;
    Shader m_lampshader;
    Mesh m_trianglemesh;
    Mesh m_pyramidmesh;
    Mesh m_cubemesh;
    Mesh m_lightmesh;
    Mesh m_lampmesh;
    
    // Light
    glm::vec3 m_lightPosition;
    glm::vec3 m_lightColor;
    glm::vec3 m_viewPosition;
    
public:
    Game();
    Game(const Game &other);
    Game &operator=(const Game &other);
    
    ~Game();
    void Execute(const std::string &resourcepath);
    void RenderPyramid();
    void RenderTriangle();
    void RenderCube();
    void RenderLamp();
    void RenderLight();
    
    //call back
    void DoKeysMovement(bool *selectedkeys);
    void DoMouseMovement(const GLfloat &mouseX, const GLfloat &mouseY);
    void DoMouseHit(const GLuint &hit, const GLuint &action);
    
};

#endif /* Game_hpp */
