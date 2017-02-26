//
//  Common.h
//  OpenGLTest
//
//  Created by GEORGE QUENTIN on 01/02/2017.
//  Copyright © 2017 GEORGE QUENTIN. All rights reserved.
//

#ifndef Common_h
#define Common_h

#include <iostream>

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

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>


#include <FreeImage.h>

#include <ctime>
#include <algorithm>
#include <assert.h>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <queue>
#include <map>
#include <set>

//#include <stdexcept>
//#include <cstdlib>
//#include <cstdio>
//#include "sys/time.h"
//#include <thread>

#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;

#endif /* Common_h */

