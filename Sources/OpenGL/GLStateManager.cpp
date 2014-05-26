#include "GLStateManager.h"
#include <OpenGL/Core>

using namespace VBGL;

void GLStateManager::setDefaultGLState()
{
    glClearColor(0, 0, 0, 1);
    glClearDepth(1);
    //glClearDepth(-1);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    //glDepthFunc(GL_GEQUAL);

    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_PROGRAM_POINT_SIZE);
}

void GLStateManager::clear(bool color, bool depth, bool stencil)
{
    GLbitfield bits = 0;
    if (color) bits |= GL_COLOR_BUFFER_BIT;
    if (depth) bits |= GL_DEPTH_BUFFER_BIT;
    if (stencil) bits |= GL_STENCIL_BUFFER_BIT;

    glClear(bits);
}

void GLStateManager::setViewport(int w, int h, int ox, int oy)
{
    glViewport(ox, oy, w, h);
}

void GLStateManager::setClearColor(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
}

void GLStateManager::setClearColor(const glm::vec3 &color, float alpha)
{
    setClearColor(color.r, color.g, color.b, alpha);
}

void GLStateManager::setWireframe()
{
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
}

void GLStateManager::setSolid()
{
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
}

void GLStateManager::enableBackClipping()
{
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    CHECK_GL_ERRORS("Enable back clipping");
}

void GLStateManager::enableFrontClipping()
{
    glCullFace(GL_FRONT);
    glEnable(GL_CULL_FACE);
    CHECK_GL_ERRORS("Enable front clipping");
}

void GLStateManager::disableClipping()
{
    glDisable(GL_CULL_FACE);
    CHECK_GL_ERRORS("Disable clipping");
}

void GLStateManager::enableDepthTest()
{
    glEnable(GL_DEPTH_TEST);
    CHECK_GL_ERRORS("Enable depth test");
}

void GLStateManager::disableDepthTest()
{
    glDisable(GL_CULL_FACE);
    CHECK_GL_ERRORS("Disable depth test");
}
