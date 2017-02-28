#ifdef _MSC_VER
#include <windows.h> // needed to get GL stuff to work
#endif
#include <SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#include "glprocs.h"
#include "vertexbuffer.h"
#include "cube.h"

VertexBuffer * generate_cube(float aXScale, float aYScale, float aZScale)
{
    VertexBuffer * vb = new VertexBuffer;
    int i;
    static GLfloat vtx[] = 
    {
     -1.0f,  1.0f,  1.0f, 
      1.0f,  1.0f,  1.0f, 
     -1.0f, -1.0f,  1.0f, 
      1.0f, -1.0f,  1.0f, 

      1.0f, -1.0f,  1.0f, 
     -1.0f, -1.0f,  1.0f, 
      1.0f, -1.0f, -1.0f, 
     -1.0f, -1.0f, -1.0f, 

     -1.0f, -1.0f, -1.0f, 
      1.0f, -1.0f, -1.0f, 
     -1.0f,  1.0f, -1.0f, 
      1.0f,  1.0f, -1.0f, 

      1.0f,  1.0f, -1.0f, 
     -1.0f,  1.0f, -1.0f, 
      1.0f,  1.0f,  1.0f, 
     -1.0f,  1.0f,  1.0f, 

     -1.0f,  1.0f,  1.0f, 
     -1.0f, -1.0f,  1.0f, 
     -1.0f,  1.0f, -1.0f, 
     -1.0f, -1.0f, -1.0f, 

      1.0f, -1.0f, -1.0f, 
      1.0f, -1.0f,  1.0f, 
      1.0f,  1.0f, -1.0f, 
      1.0f,  1.0f,  1.0f, 
    };

    static GLfloat tex[] =
    {
    0,  0,    1,  0,    0,  1,    1,  1,
    0,  0,    1,  0,    0,  1,    1,  1,
    0,  0,    1,  0,    0,  1,    1,  1,
    0,  0,    1,  0,    0,  1,    1,  1,
    0,  0,    1,  0,    0,  1,    1,  1,
    0,  0,    1,  0,    0,  1,    1,  1,

    };

    static GLuint idx[] =
    {
    0,    2,    1,
    1,    2,    3,
    5,    7,    4,
    4,    7,    6,
    8,   10,    9,
    9,   10,   11,
   12,   13,   14,
   13,   15,   14,
   16,   18,   17,
   17,   18,   19,
   20,   22,   21,
   21,   22,   23};

    vb->mPrimitiveType = GL_TRIANGLES;
    vb->mIndices = 12 * 3;
    vb->mVertices = 24;

    vb->mIndex = new GLuint[12 * 3];
    for (i = 0; i < 12 * 3; i++)
        vb->mIndex[i] = idx[i];

    vb->mNormal = new GLfloat[24 * 3];
    for (i = 0; i < 24; i++)
    {
        float l = sqrt(vtx[i * 3 + 0] * vtx[i * 3 + 0] +
                       vtx[i * 3 + 1] * vtx[i * 3 + 1] +
                       vtx[i * 3 + 2] * vtx[i * 3 + 2]);

        vb->mNormal[i * 3 + 0] = vtx[i * 3 + 0] / l;
        vb->mNormal[i * 3 + 1] = vtx[i * 3 + 1] / l;
        vb->mNormal[i * 3 + 2] = vtx[i * 3 + 2] / l;
    }

    vb->mVertex = new GLfloat[24 * 3];
    for (i = 0; i < 24; i++)
    {
        vb->mVertex[i * 3 + 0] = vtx[i * 3 + 0] * aXScale;
        vb->mVertex[i * 3 + 1] = vtx[i * 3 + 1] * aYScale;
        vb->mVertex[i * 3 + 2] = vtx[i * 3 + 2] * aZScale;
    }

    vb->mTexcoord[0] = new GLfloat[24*2];
    
    for (i = 0; i < 24 * 2; i++)
        vb->mTexcoord[0][i] = tex[i];
    
    vb->mTexcoord[1] = vb->mTexcoord[2] = vb->mTexcoord[3] = vb->mTexcoord[0];


    return vb;   
}