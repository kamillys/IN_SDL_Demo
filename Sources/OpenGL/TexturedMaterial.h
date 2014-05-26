#pragma once

//Forward declaration
class TexturedMaterial;


#include "Material.h"
#include "../mini_material.h"


class TexturedMaterial : public VBGL::Material
{
public:
    TexturedMaterial(glm::mat4& cbProj,
                     glm::mat4& cbView,
                     glm::mat4& cbModel,
                     mini::Material*& cbMaterial);
    virtual ~TexturedMaterial();

    virtual void bind();
    virtual void release();
private:
    glm::mat4& m_cbProj;
    glm::mat4& m_cbView;
    glm::mat4& m_cbModel;
    mini::Material*& m_cbMaterial;
};
