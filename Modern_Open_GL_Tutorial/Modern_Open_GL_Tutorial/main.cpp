//
//  main.cpp
//  Modern_Open_GL_Tutorial
//
//  Created by GEORGE QUENTIN on 26/08/2017.
//  Copyright © 2017 geomakesgames. All rights reserved.
//

#include "Common.h"
#include "Game.h"

int main(int argc, const char * argv[]) {
    
    srand ( time(nullptr) );
    
    static std::string resource_path = "/Users/GeorgeQuentin/Dev/OpenGL/OpenGLProject/Modern_Open_GL_Tutorial/Modern_Open_GL_Tutorial";
    
    Game game;
    game.Execute(resource_path);
    
    return 0;
}

