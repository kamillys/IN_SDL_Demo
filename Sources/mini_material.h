#pragma once

#include <glm/glm.hpp>
#include <stdexcept>
//#include "mini_dxptr.h"

namespace mini
{
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

    //const mini::dx_ptr<ID3D11ShaderResourceView>& getDiffuseTexture() { return m_diffuseTexture; }
    //void setDiffuseTexture(mini::dx_ptr<ID3D11ShaderResourceView>&& tex) { m_diffuseTexture = move(tex); }
    //const mini::dx_ptr<ID3D11ShaderResourceView>& getSpecularTexture() { return m_specularTexture; }
    //void setSpecularTexture(mini::dx_ptr<ID3D11ShaderResourceView>&& tex) { m_specularTexture = move(tex); }

private:
    Material(const Material& right) { /*do not use!*/ throw std::logic_error("Material copy constructor called!"); }
    Material& operator =(const Material& right) { /*do not use*/ throw std::logic_error("Material copy assignment called!"); }

    void Release();

    MaterialData m_data;
    //mini::dx_ptr<ID3D11ShaderResourceView> m_diffuseTexture;
    //mini::dx_ptr<ID3D11ShaderResourceView> m_specularTexture;
};
}
