#include <SDL_events.h>

#include "sdl_application.h"


SDLApplication::SDLApplication()
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);
}

SDLApplication::~SDLApplication()
{
    SDL_Quit();
}

void SDLApplication::registerWindow(GLWindowBase *win)
{
    windows.push_back(win);
}

void SDLApplication::mainLoop()
{
    initialize();
    SDL_Event e;
    bool finish = false;
    while(!finish) {
        while (SDL_PollEvent(&e)) {
            switch(e.type) {
            case SDL_QUIT: finish = true; break;
            case SDL_WINDOWEVENT: parseWindowEvent(e, e.window.windowID); break;
            case SDL_KEYUP: parseWindowEvent(e, e.key.windowID); break;
            case SDL_KEYDOWN: parseWindowEvent(e, e.key.windowID); break;
            case SDL_TEXTEDITING: parseWindowEvent(e, e.edit.windowID); break;
            case SDL_TEXTINPUT: parseWindowEvent(e, e.text.windowID); break;
            case SDL_MOUSEMOTION: parseWindowEvent(e, e.motion.windowID); break;
            case SDL_MOUSEBUTTONUP: parseWindowEvent(e, e.button.windowID); break;
            case SDL_MOUSEBUTTONDOWN: parseWindowEvent(e, e.button.windowID); break;

            case SDL_MOUSEWHEEL: parseWindowEvent(e, e.wheel.windowID); break;
            case SDL_JOYAXISMOTION: parseGlobalEvent(e); break;
            case SDL_JOYBALLMOTION: parseGlobalEvent(e); break;
            case SDL_JOYHATMOTION: parseGlobalEvent(e); break;
            case SDL_JOYBUTTONUP: parseGlobalEvent(e); break;
            case SDL_JOYBUTTONDOWN: parseGlobalEvent(e); break;
            case SDL_JOYDEVICEADDED: parseGlobalEvent(e); break;
            case SDL_JOYDEVICEREMOVED: parseGlobalEvent(e); break;
            case SDL_CONTROLLERAXISMOTION: parseGlobalEvent(e); break;
            case SDL_CONTROLLERBUTTONUP: parseGlobalEvent(e); break;
            case SDL_CONTROLLERBUTTONDOWN: parseGlobalEvent(e); break;
            case SDL_CONTROLLERDEVICEADDED: parseGlobalEvent(e); break;
            case SDL_CONTROLLERDEVICEREMOVED: parseGlobalEvent(e); break;
            case SDL_CONTROLLERDEVICEREMAPPED: parseGlobalEvent(e); break;
            case SDL_FINGERMOTION: parseGlobalEvent(e); break;
            case SDL_FINGERUP: parseGlobalEvent(e); break;
            case SDL_FINGERDOWN: parseGlobalEvent(e); break;
            case SDL_MULTIGESTURE: parseGlobalEvent(e); break;
            case SDL_DOLLARGESTURE: parseGlobalEvent(e); break;
            case SDL_DROPFILE: parseGlobalEvent(e); break;
            case SDL_SYSWMEVENT: parseGlobalEvent(e); break;
            default: parseGlobalEvent(e); break;
            }
        }
        //Event has been processed. Render
        render();
        //Then sleep
        for (int i=0; i<windows.size(); ++i)
            windows[i]->Update(0.01);
    }
}

void SDLApplication::initialize()
{
    for (int i=0; i<windows.size(); ++i)
        windows[i]->initialize();
}

void SDLApplication::render()
{
    for (int i=0; i<windows.size(); ++i)
        windows[i]->render();
}

void SDLApplication::parseGlobalEvent(const SDL_Event &event)
{
    for (int i=0; i<windows.size(); ++i)
        windows[i]->ProcessSDLEvent(event);
}

void SDLApplication::parseWindowEvent(const SDL_Event& event, Uint32 windowID)
{
    SDL_Window* window = SDL_GetWindowFromID(windowID);
    for (int i=0; i<windows.size(); ++i)
        if (windows[i]->displayWindow == window)
        {
            windows[i]->ProcessSDLEvent(event);
            break;
        }
    return;
}
