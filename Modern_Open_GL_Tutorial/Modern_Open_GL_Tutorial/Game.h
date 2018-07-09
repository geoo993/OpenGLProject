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
    Shader m_lightshader;
    Shader m_lampshader;
    
    Mesh m_pyramidmesh;
    Mesh m_cubemesh;
    Mesh m_lampmesh;
    
    // Light
    glm::vec3 m_lightColor;
    glm::vec3 m_cubesColor;
    glm::vec3 m_viewPosition;
    
    GLboolean m_useTexture;
    GLboolean m_useDir;
    GLboolean m_usePoint;
    GLboolean m_useSpot;
    
    
    vector<glm::vec3> m_cubesPosition = {
        glm::vec3(-1.0f, -4.0f, -1.0f),
        glm::vec3(-8.0f, 7.0f, 5.0f),
        glm::vec3(-5.0f, 3.0f, -2.0f),
        glm::vec3(2.0f, 5.0f, 8.0f),
        glm::vec3(-2.0f, 8.0f, -9.0f),
        glm::vec3(4.0f, -1.0f, -2.0f),
        glm::vec3(9.0f, -5.0f, 3.0f),
        glm::vec3(-8.0f, 2.0f, 8.0f)
    };
    
    vector<glm::vec3> m_directionalLightsDirections = {
        glm::vec3(3.2f, 3.0f, -0.3f),
        glm::vec3(  15.7f,  5.2f,  -6.0f      ),
        glm::vec3(  -9.3f, -14.3f, -2.0f      ),
        glm::vec3(  -24.0f,  9.0f, -15.0f    ),
    };
    
    GLuint m_pointLightPositionsIndex = 0;
    vector<glm::vec3> m_pointLightPositions = {
        glm::vec3(  1.0f,  3.0f,  -2.0f      ),
        glm::vec3(  -5.7f,  0.2f,  2.0f      ),
        glm::vec3(  2.3f, -3.3f, -4.0f      ),
        glm::vec3(  -4.0f,  2.0f, -12.0f    ),
        glm::vec3(  0.0f,  0.0f, -3.0f      )
    };
    
    std::vector<glm::vec3> m_pointlightsColours = {
        glm::vec3(1.0f, 1.0f, 0.0f),
        glm::vec3(0.4f, 0.6f, 0.7f),
        glm::vec3(0.9f, 0.1f, 0.4f),
        glm::vec3(0.6f, 0.2f, 0.25f),
        glm::vec3(0.3f, 0.5f, 0.2f),
    };

    // inputs
    double m_keyPressTime;
    double m_lastKeyPressTime;
    
public:
    Game();
    Game(const Game &other);
    Game &operator=(const Game &other);
    ~Game();
    
    void Execute(const std::string &resourcepath);
    void RenderPyramid();
    void RenderCube();
    void RenderCubes();
    void RenderLamp();
    
    //call back
    void DoKeysMovement(bool *selectedkeys);
    void DoMouseMovement(const GLfloat &mouseX, const GLfloat &mouseY);
    void DoMouseHit(const GLuint &hit, const GLuint &action);
    
};

#endif /* Game_hpp */
