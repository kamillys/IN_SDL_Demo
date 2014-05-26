#include "mini_material.h"

using namespace std;
using namespace mini;
using namespace glm;

Material::Material()
    /*: m_diffuseTexture(nullptr), m_specularTexture(nullptr)*/
{
    m_data.diffuseColor = vec4(0, 0, 0, 0);
    m_data.specularColor = vec4(0, 0, 0, 1);
}

void Material::Release()
{
    //TODO: Consider if move is required
    //m_diffuseTexture.reset();
    //m_specularTexture.reset();
    m_data.diffuseColor = vec4(0, 0, 0, 0);
    m_data.specularColor = vec4(0, 0, 0, 1);
}

Material::Material(Material&& right)
    : m_data(right.m_data), m_diffuseTexture(move(right.m_diffuseTexture)), m_specularTexture(move(right.m_specularTexture))
{
	right.Release();
}

Material& Material::operator =(Material&& right)
{
	m_data.diffuseColor = right.m_data.diffuseColor;
    m_data.specularColor = right.m_data.specularColor;
    m_diffuseTexture = move(right.m_diffuseTexture);
    m_specularTexture = move(right.m_specularTexture);
	right.Release();
	return *this;
}
