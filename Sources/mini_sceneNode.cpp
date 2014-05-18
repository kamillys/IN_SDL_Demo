#include "mini_sceneNode.h"

using namespace mini;
using namespace glm;

SceneNode::SceneNode()
        : m_firstChild(-1), m_nextSybling(-1), m_parent(-1), m_mesh(-1)
{
}
