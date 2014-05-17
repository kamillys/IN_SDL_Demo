#include "sdl_application.h"

#include <SDL_events.h>

SDLApplication::SDLApplication()
{
    SDL_Init(SDL_INIT_VIDEO);
}

SDLApplication::~SDLApplication()
{
    SDL_Quit();
}

void SDLApplication::registerWindow(GLWindowBase *win)
{

}

void SDLApplication::mainLoop()
{
    SDL_Event e;
    bool finish = false;
    while(!finish) {
        while (SDL_PollEvent(&e)) {
            switch(e.type) {
            case SDL_QUIT: finish = true; break;
            case SDL_MOUSEMOTION: break;
            case SDL_MOUSEBUTTONUP: break;
            case SDL_MOUSEBUTTONDOWN: break;
            case SDL_WINDOWEVENT: ProcessWindowEvent(e); parseWindowEvent(&e); break;
            default: fprintf (stderr, "EVENT: %i\n", e.type);
            }
        }
        //Event has been processed. Render
        render();
        //Then sleep
    }
}

static void parseWindowEvent(SDL_Event* event)
{
    switch (event->window.event) {
    case SDL_WINDOWEVENT_SHOWN:
        SDL_Log("Window %d shown", event->window.windowID);
        break;
    case SDL_WINDOWEVENT_HIDDEN:
        SDL_Log("Window %d hidden", event->window.windowID);
        break;
    case SDL_WINDOWEVENT_EXPOSED:
        SDL_Log("Window %d exposed", event->window.windowID);
        break;
    case SDL_WINDOWEVENT_MOVED:
        SDL_Log("Window %d moved to %d,%d",
                event->window.windowID, event->window.data1,
                event->window.data2);
        break;
    case SDL_WINDOWEVENT_RESIZED:
        SDL_Log("Window %d resized to %dx%d",
                event->window.windowID, event->window.data1,
                event->window.data2);
        break;
    case SDL_WINDOWEVENT_MINIMIZED:
        SDL_Log("Window %d minimized", event->window.windowID);
        break;
    case SDL_WINDOWEVENT_MAXIMIZED:
        SDL_Log("Window %d maximized", event->window.windowID);
        break;
    case SDL_WINDOWEVENT_RESTORED:
        SDL_Log("Window %d restored", event->window.windowID);
        break;
    case SDL_WINDOWEVENT_ENTER:
        SDL_Log("Mouse entered window %d",
                event->window.windowID);
        break;
    case SDL_WINDOWEVENT_LEAVE:
        SDL_Log("Mouse left window %d", event->window.windowID);
        break;
    case SDL_WINDOWEVENT_FOCUS_GAINED:
        SDL_Log("Window %d gained keyboard focus",
                event->window.windowID);
        break;
    case SDL_WINDOWEVENT_FOCUS_LOST:
        SDL_Log("Window %d lost keyboard focus",
                event->window.windowID);
        break;
    case SDL_WINDOWEVENT_CLOSE:
        SDL_Log("Window %d closed", event->window.windowID);
        break;
    default:
        SDL_Log("Window %d got unknown event %d",
                event->window.windowID, event->window.event);
        break;
    }

}
