#ifdef _MSC_VER
#include <windows.h> // needed to get GL stuff to work
#endif
#include <SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#include "glprocs.h"

GLuint load_texture(char * aFilename)
{
    GLuint texname;
    glGenTextures(1,&texname);
    glBindTexture(GL_TEXTURE_2D,texname);
    SDL_Surface *temp = SDL_LoadBMP(aFilename);
    SDL_PixelFormat pf;
    pf.palette = NULL;
    pf.BitsPerPixel = 32;
    pf.BytesPerPixel = 4;
    pf.alpha = 0;
    pf.colorkey = 0;
    pf.Rmask = 0x000000ff;
    pf.Rshift = 0;
    pf.Rloss = 0;
    pf.Gmask = 0x0000ff00;
    pf.Gshift = 8;
    pf.Gloss = 0;
    pf.Bmask = 0x00ff0000;
    pf.Bshift = 16;
    pf.Bloss = 0;
    pf.Amask = 0xff000000;
    pf.Ashift = 24;
    pf.Aloss = 0;
    
    SDL_Surface *tm = SDL_ConvertSurface(temp, &pf,SDL_SWSURFACE);
    SDL_FreeSurface(temp);
    SDL_LockSurface(tm);
    glTexImage2D(GL_TEXTURE_2D,0,3,tm->w,tm->h,0,GL_RGBA,GL_UNSIGNED_BYTE,(GLvoid*)tm->pixels);
    SDL_UnlockSurface(tm);
    
    SDL_FreeSurface(tm);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); // Linear Filtering
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // Linear Filtering

    return texname;
}
