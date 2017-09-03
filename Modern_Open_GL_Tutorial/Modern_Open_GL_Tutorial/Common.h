//
//  Common.h

//  Created by GEORGE QUENTIN on 01/02/2017.
//  Copyright © 2017 GEORGE QUENTIN. All rights reserved.
//

#ifndef Common_h
#define Common_h

//Mac OpenGL
//#include <OpenGL/gl.h>
//#include <OpenGl/glu.h>
//#include <OpenGL/OpenGL.h>
//#include <OpenGL/gl3.h>

#define GLEW_STATIC

//GLEW
#include <GL/glew.h>//include GLEW and new version of GL on Windows


#define GLFW_INCLUDE_GL3  /* don't drag in legacy GL headers. */
#define GLFW_NO_GLU       /* don't drag in the old GLU lib - unless you must. */
//GLFW
#include <GLFW/glfw3.h>// GLFW helper library

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/matrix_decompose.hpp>

//FreeImage
#include <FreeImage.h>


#include <iostream>
#include <algorithm>
#include <assert.h>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <queue>
#include <map>
#include <set>
#include <stack>
#include <cstdlib>
#include <cstdio>
#include "sys/time.h"

#define _USE_MATH_DEFINES
#include <math.h>

#define SCREEN_WIDTH 1020
#define SCREEN_HEIGHT 720

using namespace std;

static std::string RESOURCE_PATH = "/Users/GeorgeQuentin/Dev/OpenGL/OpenGLProject/Modern_Open_GL_Tutorial/Modern_Open_GL_Tutorial";

#endif /* Common_h */

