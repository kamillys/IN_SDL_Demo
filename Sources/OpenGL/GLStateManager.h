#pragma once

#include <glm/glm.hpp>

namespace VBGL
{
namespace GLStateManager
{

void setDefaultGLState();

void clear(bool color = true, bool depth = true, bool stencil = true);
void setViewport(int w, int h, int ox = 0, int oy = 0);
void setClearColor(float r, float g, float b, float a = 1);
void setClearColor(const glm::vec3& color, float alpha = 1);
void setWireframe();
void setSolid();
void enableBackClipping();
void enableFrontClipping();
void disableClipping();

void enableDepthTest();
void disableDepthTest();

}
}
