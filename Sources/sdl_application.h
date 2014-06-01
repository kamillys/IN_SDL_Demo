#pragma once


//Forward declaration
class SDLApplication;

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <vector>

#include "window_base.h"


class SDLApplication
{
    SDLApplication(const SDLApplication&) = delete;
    friend class GLWindowBase;

    std::vector<GLWindowBase*> windows; //xD

    void registerWindow(GLWindowBase* win);
    void parseWindowEvent(const SDL_Event &event, Uint32 windowID);
    void parseGlobalEvent(const SDL_Event &event);
    void initialize();
    void render();
    void update(double T);
public:
    SDLApplication();
    ~SDLApplication();
    void mainLoop();
};
