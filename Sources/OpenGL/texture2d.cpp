#include "texture2d.h"

using namespace VBGL;

Texture2D::Texture2D()
{
    glGenTextures(1, &texID);
    CHECK_GL_ERRORS("Texture2D generate");
}

Texture2D::~Texture2D()
{
    release();
    glDeleteTextures(1, &texID);
    CHECK_GL_ERRORS("Texture2D deleting");
}

void Texture2D::bind()
{
    glBindTexture(GL_TEXTURE_2D, texID);
    CHECK_GL_ERRORS("Texture2D binding");
}

void Texture2D::release()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::setWrap(GLint horizontal, GLint vertical)
{
    bind();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, horizontal);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, vertical);
    CHECK_GL_ERRORS("Texture2D setWrap");
}

void Texture2D::setMagFilter(GLint type)
{
    bind();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, type);
    CHECK_GL_ERRORS("Texture2D setMagFilter");
}

void Texture2D::setMinFilter(GLint type)
{
    bind();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, type);
    CHECK_GL_ERRORS("Texture2D setMinFilter");
}

void Texture2D::generateMipmap()
{
    bind();
    glGenerateMipmap(GL_TEXTURE_2D);
    CHECK_GL_ERRORS("Texture2D generateMipmap");
}

void Texture2D::setDefaultParameters()
{
    setWrap(GL_CLAMP, GL_CLAMP);
    setMagFilter(GL_NEAREST);
    setMinFilter(GL_NEAREST);
    generateMipmap();
}

#include <SDL2/SDL_image.h>
#include <vector>

VTF::RefPointer<Texture2D> Texture2D::loadTexture(const char *path)
{
    SDL_Surface* surface = IMG_Load(path);
    if (!surface)
        THROW_EXCEPTION("Cannot load image!");
    SDL_FreeSurface(surface);

    GLenum Mode = 0;
    if (!surface->format) {
        Mode = GL_RGBA;//Set default
    } else {
        switch (surface->format->BytesPerPixel) {
        case 1: {
            Mode = GL_ALPHA;
            break;
        }
        case 3: {
            Mode = GL_RGB;
            break;
        }
        case 4: {
            Mode = GL_RGBA;
            break;
        }
        default: { break;
        }
        }
    }

    std::vector<u_int8_t> pixels(surface->w * surface->h);
    for (int i=0;i<surface->h;++i)
    {
        memcpy(pixels.data()+i*surface->w, surface->pixels+i*surface->pitch, surface->w);
    }

    VTF::RefPointer<Texture2D> texture = new Texture2D;
    texture->bind();

    //glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    //glPixelStorei(GL_UNPACK_ROW_LENGTH,surface->pitch);
    glTexImage2D(GL_TEXTURE_2D, 0, Mode, surface->w, surface->h, 0, Mode, GL_UNSIGNED_BYTE, pixels.data());

    texture->setDefaultParameters();
    texture->setWrap(GL_REPEAT, GL_REPEAT);

    return texture;
}
