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
    
    Game game;
    game.Execute(RESOURCE_PATH);
    
    return 0;
}

