#pragma once

#include <glm/glm.hpp>
#include <string>

namespace mini
{
struct SceneNode
{
    SceneNode();

    glm::mat4 m_localTransform;
    glm::mat4 m_transform;
    std::string m_name;
    int m_nextSybling;
    int m_firstChild;
    int m_parent;
    int m_mesh;
};
}
