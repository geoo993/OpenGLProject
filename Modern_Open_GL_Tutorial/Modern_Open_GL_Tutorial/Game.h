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
#include "Transform.h"
#include "Texture.h"
#include "Camera.h"
#include "Shader.h"
#include "Mesh.h"

class Game {
private:
    
    GLFWwindow* m_window;

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
    Mesh m_trianglemesh;
    Mesh m_pyramidmesh;
    Mesh m_cubemesh;
    Camera m_camera;
    Texture m_texture;
    
public:
    Game();
    Game(const Game &other);
    Game &operator=(const Game &other);
    
    ~Game();
    void Execute(const std::string &resourcepath);
    void RenderPyramid();
    void RenderTriangle();
    void RenderCube();
    void DoMovement();
    
};

#endif /* Game_hpp */
