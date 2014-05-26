#pragma once

#include "myGL.h"

namespace VBGL
{

class GLCaps
{
public:
    GLCaps();

#include "GLExts.inl"

#define X(extension) \
    bool has_##extension() \
    { return m_has_##extension; }

    GLCAPS_EXTENSION_LIST(X)
#undef X

    void printExtensions();

//TODO: Renamed & helper caps
    bool hasGeometryShader()
    { return has_GL_ARB_geometry_shader4() || has_GL_EXT_geometry_shader4() || has_GL_NV_geometry_shader4(); }

    bool hasTessellationShader() {
        return has_GL_ARB_tessellation_shader();
    }

#define GLCAPS_INTEGER_DATA(X)\
    X(maxTextureSize, GL_MAX_TEXTURE_SIZE) \
    X(max3DTextureSize, GL_MAX_3D_TEXTURE_SIZE) \
    X(maxCubeMapSize, GL_MAX_CUBE_MAP_TEXTURE_SIZE) \
    X(maxRenderBufferSize, GL_MAX_RENDERBUFFER_SIZE)

#define X(name, cap) \
    GLint name() { \
        GLint retval; glGetIntegerv(cap, &retval);\
        return retval; \
    }
    GLCAPS_INTEGER_DATA(X)
#undef X
#undef GLCAPS_INTEGER_DATA

private:
#define X(extension) \
    bool m_has_##extension;

    GLCAPS_EXTENSION_LIST(X)
#undef X
};

}
