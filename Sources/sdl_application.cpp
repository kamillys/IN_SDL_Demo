#include <SDL_events.h>
#include <chrono>
#include <thread>
#include <SDL_ttf.h>
#include "exception.h"
#include "sdl_application.h"


SDLApplication::SDLApplication()
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);
    if (TTF_Init() == -1)
        THROW_EXCEPTION("Cannot load TTF!");
}

SDLApplication::~SDLApplication()
{
    TTF_Quit();
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
    auto nowTime = std::chrono::high_resolution_clock::now();
    auto prevTime = std::chrono::high_resolution_clock::now();
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
        //Events has been processed.
        //Compute delta time
        nowTime = std::chrono::high_resolution_clock::now();
        auto deltaTime = nowTime - prevTime;
        std::chrono::duration<double> elapsed_seconds = deltaTime;
        auto deltaTimeMS = std::chrono::duration_cast<std::chrono::milliseconds>(deltaTime).count();
        prevTime = nowTime;

        //Update
        update(elapsed_seconds.count());
        //Render
        render();
        //Then sleep
        if (deltaTimeMS < 20)
            std::this_thread::sleep_for(std::chrono::milliseconds(20 - deltaTimeMS));
    }
}

void SDLApplication::initialize()
{
    for (int i=0; i<windows.size(); ++i)
        windows[i]->initialize();
}

void SDLApplication::update(double T)
{
    for (int i=0; i<windows.size(); ++i)
        windows[i]->Update(T);
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
