#include <memory>

#include "OpenGL/myGL.h"
#include "sdl_application.h"
#include "exception.h"
#include "in_scene.h"
#include "util.h"

int main(int argc, char *argv[])
{
    SDLApplication sdlapp;
    std::unique_ptr<INScene> app;
    int exitCode = 0;
    try
    {
        app.reset(new INScene(&sdlapp, 1280, 720));

        sdlapp.mainLoop();
    }
    catch (VTF::Exception& e)
    {
        DebugLogV(e.getMessage().c_str());
        exitCode = e.getExitCode();
    }
    catch (std::exception& e)
    {
        DebugLogV(e.what());
        exitCode = -1;
    }
    catch(const char* str)
    {
        DebugLogV(str);
        exitCode = -1;
    }
    catch(const wchar_t* str)
    {
        std::wstring ws( str );
        std::string s( ws.begin(), ws.end() );
        DebugLogV(s.c_str());
        exitCode = -1;
    }
    catch(...)
    {
        DebugLogV("Unknown error");
        exitCode = -1;
    }
    app.reset();
    return exitCode;
}
