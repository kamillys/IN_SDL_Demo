#pragma once


//Forward declaration
class GLWindowBase;

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include "sdl_application.h"

class GLWindowBase
{
    friend class SDLApplication;
    SDL_Window* displayWindow;
    SDL_GLContext openglContext;
    int _w;
    int _h;
public:
    GLWindowBase(SDLApplication* app, int width, int height);
    virtual ~GLWindowBase();

    void setTitle(const char* title);

    int width() const;
    int height() const;
protected:
    virtual void resizeGL() = 0;
    virtual void initGL() = 0;
    virtual void displayGL() = 0;

    virtual void ProcessEvent(const SDL_Event& event);
    virtual void Update(float dt);
private:
    void initialize();
    void render();
    void ProcessSDLEvent(const SDL_Event& event);
    void ProcessWindowEvent(const SDL_Event& event);
};
