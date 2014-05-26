#pragma once

//Forward declaration
namespace mini {
class Material;
}

#include <glm/glm.hpp>
#include <stdexcept>
#include "OpenGL/texture2d.h"

//#include "mini_dxptr.h"

namespace mini
{
//TODO: Whole thing
class Material
{
public:
    struct MaterialData
    {
        glm::vec4 diffuseColor; //[r, g, b, a]
        glm::vec4 specularColor; //[r, g, b, m]
    };

    Material();
    Material(Material&& right);
    Material& operator =(Material&& right);

    const MaterialData& getMaterialData() const { return m_data; }
    void setMaterialData(const MaterialData& data) { m_data = data; }

    VTF::RefPointer<VBGL::Texture2D>& getDiffuseTexture() { return m_diffuseTexture; }
    void setDiffuseTexture(VTF::RefPointer<VBGL::Texture2D>&& tex) { m_diffuseTexture = std::move(tex); }
    VTF::RefPointer<VBGL::Texture2D>& getSpecularTexture() { return m_specularTexture; }
    void setSpecularTexture(VTF::RefPointer<VBGL::Texture2D>&& tex) { m_specularTexture = std::move(tex); }

private:
    Material(const Material& right) = delete; //{ /*do not use!*/ throw std::logic_error("Material copy constructor called!"); }
    Material& operator =(const Material& right) = delete; // { /*do not use*/ throw std::logic_error("Material copy assignment called!"); }

    void Release();

    MaterialData m_data;
    VTF::RefPointer<VBGL::Texture2D> m_diffuseTexture;
    VTF::RefPointer<VBGL::Texture2D> m_specularTexture;
};
}
