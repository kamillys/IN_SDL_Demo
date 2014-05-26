#pragma once

//#include "mini_dxptr.h"
#include <vector>
#include <string>
#include <stdexcept>
#include <glm/glm.hpp>

#include "OpenGL/Geometry.h"

namespace mini
{
class Mesh
{
public:
    Mesh();
    Mesh(std::vector<glm::vec3>&& positions,
         std::vector<glm::vec2>&& texCoords,
         std::vector<unsigned short>&& pindices,
         std::vector<glm::vec3>&& normals,
         unsigned int materialIdx);

    Mesh(Mesh&& right);
    ~Mesh();

    Mesh& operator=(Mesh&& right);
    void Render(VBGL::ShaderProgram* shader/*const mini::dx_ptr<ID3D11DeviceContext>& context*/);
    const glm::mat4& getTransform() const { return m_transform; }
    void setTransform(const glm::mat4& transform) { m_transform = transform; }

    unsigned int getMaterialIdx() const { return m_materialIdx; }
    void setMaterialIdx(unsigned int idx) { m_materialIdx = idx; }

private:
    Mesh(const Mesh& right) = delete;
    //{/*do not use*/ throw std::logic_error("Mesh copy constructor should never be called!"); }

    Mesh& operator=(const Mesh& right) = delete;
    //{ /*do not use*/ throw std::logic_error("Mesh copy assignment should never be called!"); }
    void Release();

    VTF::RefPointer<VBGL::Geometry> m_geometry;
    glm::mat4 m_transform;
    unsigned int m_materialIdx;
};
}
