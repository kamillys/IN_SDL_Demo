#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "window_base.h"
#include "test_window.h"
#include "sdl_application.h"

int main(int argc, char *argv[])
{
    SDLApplication app;
    {
        //TestWindow win(800, 600);
        TestWindow2 win(&app);

        app.mainLoop();

        win.initialize();
        win.MainLoop();
        fflush(stdout);
    }
    SDL_Log("Close?");
    SDL_Delay(500);
    return 0;
}
