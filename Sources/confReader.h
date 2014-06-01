#pragma once

#include <SDL.h>

class ConfigReader
{
public:
    SDL_Scancode getKeyMap(const char* name, const char* def);
};
