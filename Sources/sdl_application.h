#pragma once


//Forward declaration
class SDLApplication;

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include "window_base.h"

class SDLApplication
{
    SDLApplication(const SDLApplication&) = delete;
    friend class GLWindowBase;

    void registerWindow(GLWindowBase* win);
public:
    SDLApplication();
    ~SDLApplication();
    void mainLoop();
};
