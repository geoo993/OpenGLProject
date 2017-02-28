#ifdef _MSC_VER
#include <windows.h> // needed to get GL stuff to work
#endif
#include <SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "glprocs.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "vertexbuffer.h"
#include "torusknot.h"
#include "cube.h"
#include "loadtexture.h"

static void handle_key_down(SDL_keysym* keysym)
{
    switch (keysym->sym) 
    {
    case SDLK_ESCAPE:
        SDL_Quit();
        exit(0);
        break;
    default:
        break;
    }
}

static void process_events()
{
    SDL_Event event;

    while (SDL_PollEvent(&event)) 
    {
        switch (event.type) 
        {
        case SDL_KEYDOWN:
            handle_key_down(&event.key.keysym);
            break;
        case SDL_QUIT:
            SDL_Quit();
            exit(0);
            break;
        }
    }
}

GLuint texturemap;

void torusknot()
{
    float angle = SDL_GetTicks() / 10000.0f;
    
    VertexBuffer *vb = generate_torusknot(
                       256,
                       16,
                       (float)(1.5f + sin(angle) / 2),
                       0.2f,
                       12,
                       angle * 30,
                       0.5f,
                       4,
                       64,
                       2,
                       5);
    VertexBuffer *vb2 = generate_cube(50,50,50);

//    glClearColor(0.0,0.0,1.0,1.0);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );


    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity( );

    glTranslatef( 0.0, 0.0, -5.0 );

    glRotatef( angle * 360, 0.4f, 1.0f, 0.2f);

    vb2->activate();

    glBindTexture(GL_TEXTURE_2D,texturemap);
    glEnable(GL_TEXTURE_2D);
    glFrontFace(GL_CW);
    vb2->render();
    glFrontFace(GL_CCW);

    vb->activate();

    glEnable(GL_TEXTURE_2D);

    vb->render();
    vb->render_normals(0.1f);
    // render_normals messes up the vertex pointer, so we'll need to reactivate
	vb->activate(); 

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    GLfloat lightpos[4];

    lightpos[0] = (float)sin(angle * 10) * 10;
    lightpos[1] = (float)cos(angle * 10) * 10;
    lightpos[2] = (float)cos(angle * 5) * 10;
    lightpos[3] = 1;
    glLightfv(GL_LIGHT0, GL_POSITION, lightpos); // 4 floats
    lightpos[0] = 1;
    lightpos[1] = 1;
    lightpos[2] = 1;
    lightpos[3] = 1;
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightpos);

    glEnable(GL_BLEND);
    glBlendFunc(GL_DST_COLOR,GL_SRC_COLOR);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    vb->render();

    glDisable(GL_BLEND);
    glDisable(GL_LIGHTING);

    delete vb;
	delete vb2;

    SDL_GL_SwapBuffers();
}

static void draw_screen()
{
    torusknot();
}



static void setup_opengl( int width, int height )
{
    float ratio = (float) width / (float) height;

    glShadeModel( GL_SMOOTH );

    glCullFace( GL_BACK );
    glFrontFace( GL_CCW );
    glEnable( GL_CULL_FACE );

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glClearColor( 0, 0, 0, 0 );

    glViewport( 0, 0, width, height );

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity( );
    gluPerspective( 60.0, ratio, 1.0, 1024.0 );

    texturemap = load_texture("texture.bmp");

}

int main (int argc, char** args)
{
    const SDL_VideoInfo *info = NULL;
    int width = 0;
    int height = 0;
    int bpp = 0;
    int flags = 0;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE) < 0) 
    {
        fprintf(stderr, "Video initialization failed: %s\n", SDL_GetError());
        SDL_Quit();
        exit(0);
    }

    info = SDL_GetVideoInfo();

    if (!info) 
    {
        fprintf(stderr, "Video query failed: %s\n", SDL_GetError());
        SDL_Quit();
        exit(0);
    }

    width = 640;
    height = 480;
    bpp = info->vfmt->BitsPerPixel;

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    flags = SDL_OPENGL;/* | SDL_FULLSCREEN;*/

    if (SDL_SetVideoMode(width, height, bpp, flags) == 0) 
    {
        fprintf( stderr, "Video mode set failed: %s\n", SDL_GetError());
        SDL_Quit();
        exit(0);
    }

    setup_opengl(width, height);

    while (1) 
    {
        process_events();
        draw_screen();
    }

    return 0;
}
