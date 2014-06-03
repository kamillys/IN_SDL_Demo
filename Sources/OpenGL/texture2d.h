#pragma once

#include <VTF/Pointers>
#include <SDL.h>
#include "myGL.h"

namespace VBGL
{

class Texture2D : public VTF::RefObject
{
    GLuint texID;
public:
    Texture2D();
    ~Texture2D();

    void bind();
    void release();

    void setWrap(GLint horizontal, GLint vertical);
    void setMagFilter(GLint type);
    void setMinFilter(GLint type);
    void generateMipmap();

    void setDefaultParameters();

    static VTF::RefPointer<Texture2D> loadTexture(SDL_Surface* surface);
    static VTF::RefPointer<Texture2D> loadTexture(const char* path);
    static inline VTF::RefPointer<Texture2D> loadTexture(const std::string& path)
    {
        return loadTexture(path.c_str());
    }
};

}
