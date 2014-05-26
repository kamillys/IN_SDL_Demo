#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <unordered_map>
//#include "mini_dxptr.h"
#include "mini_mesh.h"
#include "mini_sceneNode.h"
#include "mini_material.h"

namespace mini
{
class SceneGraph
{
public:
    SceneGraph(std::vector<mini::SceneNode>&& nodes,
               std::vector<mini::Mesh>&& meshes,
               std::vector<mini::Material>&& materials);
    SceneGraph(SceneGraph&& right);

    SceneGraph& operator=(SceneGraph&& right);

    int nodeByName(const std::string& name) const;
    glm::mat4 getNodeTransform(unsigned nodeIndex) const;
    void setNodeTransform(unsigned int nodeIndex, const glm::mat4& transform);
    int nodeFirstChild(unsigned int nodeIndex) const;
    int nodeNextSybling(unsigned int nodeIndex) const;
    int nodeParent(unsigned int nodeIndex) const;

    unsigned int meshCount() const { return m_meshes.size(); }
    Mesh& getMesh(unsigned int index)
    {
        return m_meshes.at(index);
    }
    mini::Material& getMaterial(unsigned int index) { return m_materials.at(index); }

private:
    SceneGraph(const SceneGraph& right)
    { /*do not use*/ throw std::logic_error("SceneGraph copy constructor called!"); }
    SceneGraph& operator=(const SceneGraph& right)
    { /*do not use*/ throw std::logic_error("SceneGraph copy assignment called!"); }
    void UpdateChildTransforms(unsigned int childIdx, glm::mat4 parentTransform);

    void Clear();

    std::vector<mini::Mesh> m_meshes;
    std::vector<mini::SceneNode> m_nodes;
    std::vector<mini::Material> m_materials;

};
}
