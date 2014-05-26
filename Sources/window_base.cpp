#include <glew.h>

#include "window_base.h"
#include "util.h"

GLWindowBase::GLWindowBase(SDLApplication *app, int width, int height) :
    displayWindow(nullptr),
    _w(width), _h(height)
{
    //Use OpenGL 3.0 core
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 0 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );

    //Use OpenGL 2.1
    //SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 2 );
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

    app->registerWindow(this);


    glewExperimental = GL_FALSE;
    //glewExperimental = GL_TRUE;
    GLenum glew_result = glewInit();
    assert(glew_result == GLEW_OK);
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

void GLWindowBase::ProcessEvent(const SDL_Event &event)
{
}

void GLWindowBase::Update(float dt)
{
}

void GLWindowBase::ProcessSDLEvent(const SDL_Event &event)
{
    switch(event.type)
    {
    case SDL_WINDOWEVENT: ProcessWindowEvent(event); break;
    default: ProcessEvent(event);
    }
}

void GLWindowBase::ProcessWindowEvent(const SDL_Event &event)
{
    switch (event.window.event) {
    case SDL_WINDOWEVENT_SHOWN:
        SDL_Log("Window %d shown", event.window.windowID);
        break;
    case SDL_WINDOWEVENT_HIDDEN:
        SDL_Log("Window %d hidden", event.window.windowID);
        break;
    case SDL_WINDOWEVENT_EXPOSED:
        SDL_Log("Window %d exposed", event.window.windowID);
        break;
    case SDL_WINDOWEVENT_MOVED:
        SDL_Log("Window %d moved to %d,%d", event.window.windowID, event.window.data1, event.window.data2);
        break;
    case SDL_WINDOWEVENT_RESIZED:
        resizeGL();
        SDL_Log("Window %d resized to %dx%d", event.window.windowID, event.window.data1, event.window.data2);
        break;
    case SDL_WINDOWEVENT_MINIMIZED:
        SDL_Log("Window %d minimized", event.window.windowID);
        break;
    case SDL_WINDOWEVENT_MAXIMIZED:
        SDL_Log("Window %d maximized", event.window.windowID);
        break;
    case SDL_WINDOWEVENT_RESTORED:
        SDL_Log("Window %d restored", event.window.windowID);
        break;
    case SDL_WINDOWEVENT_ENTER:
        SDL_Log("Mouse entered window %d", event.window.windowID);
        break;
    case SDL_WINDOWEVENT_LEAVE:
        SDL_Log("Mouse left window %d", event.window.windowID);
        break;
    case SDL_WINDOWEVENT_FOCUS_GAINED:
        SDL_Log("Window %d gained keyboard focus", event.window.windowID);
        break;
    case SDL_WINDOWEVENT_FOCUS_LOST:
        SDL_Log("Window %d lost keyboard focus", event.window.windowID);
        break;
    case SDL_WINDOWEVENT_CLOSE:
        SDL_Log("Window %d closed", event.window.windowID);
        break;
    default:
        SDL_Log("Window %d got unknown event %d", event.window.windowID, event.window.event);
        break;
    }
}
