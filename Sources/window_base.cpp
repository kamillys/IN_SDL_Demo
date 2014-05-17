#include "window_base.h"
#include "util.h"

GLWindowBase::GLWindowBase(SDLApplication *app, int width, int height) :
    displayWindow(nullptr),
    _w(width), _h(height)
{
    //Use OpenGL 3.1 core
    //SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );
    //SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );

    //Use OpenGL 2.1
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 2 );
    //SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );

    displayWindow = SDL_CreateWindow( "", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
    assert(displayWindow);
    openglContext = SDL_GL_CreateContext( displayWindow );
    assert(openglContext);

    //Use Vsync
    if( SDL_GL_SetSwapInterval( 1 ) < 0 )
    {
        printf( "Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError() );
    }
}

GLWindowBase::~GLWindowBase()
{
    fprintf(stderr, "Destroying window\n");
    SDL_DestroyWindow(displayWindow);
}

void GLWindowBase::setTitle(const char *title)
{
    SDL_SetWindowTitle(displayWindow, title);
}

int GLWindowBase::width() const
{
    return _w;
}

int GLWindowBase::height() const
{
    return _h;
}

void GLWindowBase::initialize()
{
    initGL();
    resizeGL();
}

void GLWindowBase::render()
{
    displayGL();
    SDL_GL_SwapWindow(displayWindow);
}
