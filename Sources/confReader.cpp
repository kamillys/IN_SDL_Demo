#include "confReader.h"
#include <string>

#include "exception.h"

SDL_Scancode ConfigReader::getKeyMap(const char *name, const char *def)
{
    if (std::string("MoveUp") == name)
        return SDL_SCANCODE_W;
    if (std::string("MoveDown") == name)
        return SDL_SCANCODE_S;
    if (std::string("MoveLeft") == name)
        return SDL_SCANCODE_A;
    if (std::string("MoveRight") == name)
        return SDL_SCANCODE_D;
    if (std::string("Action") == name)
        return SDL_SCANCODE_E;

    THROW_EXCEPTION("INVALID ACTION!");
}
