#include "TexturedMaterial.h"
#include "../util.h"
#include "gl_utils.h"

TexturedMaterial::TexturedMaterial(glm::mat4 &cbProj, glm::mat4 &cbView, glm::mat4 &cbModel, mini::Material* &cbMaterial) :
    m_cbProj(cbProj),
    m_cbView(cbView),
    m_cbModel(cbModel),
    m_cbMaterial(cbMaterial)
{
    shader = new VBGL::ShaderProgram;

    shader->addShader(loadFile("textured_vs.glsl").c_str(), VBGL::ShaderProgram::VertexShader);
    shader->addShader(loadFile("textured_fs.glsl").c_str(), VBGL::ShaderProgram::FragmentShader);

    shader->build();
}

TexturedMaterial::~TexturedMaterial()
{
}

void TexturedMaterial::bind()
{
    glm::mat4 mvp = m_cbProj * m_cbView * m_cbModel;
    glm::mat4 itmvp = glm::inverseTranspose(mvp);
    shader->setUniform("UView", m_cbView);
    shader->setUniform("UModelView", m_cbView * m_cbModel);
    shader->setUniform("UMVP", mvp);
    shader->setUniform("UNormalMatrix", itmvp);
    shader->setUniform("diffuseColor", m_cbMaterial->getMaterialData().diffuseColor);
    shader->setUniform("specularColor", m_cbMaterial->getMaterialData().specularColor);

    VBGL::Texture2D* diffTex = m_cbMaterial->getDiffuseTexture();
    VBGL::Texture2D* specTex = m_cbMaterial->getSpecularTexture();

    if (diffTex)
    {
        shader->setUniform("diffuseTex", 0);
        glActiveTexture( GL_TEXTURE0 );
        diffTex->bind();
    }
    if (specTex)
    {
        shader->setUniform("specularTex", 1);
        glActiveTexture( GL_TEXTURE1 );
        specTex->bind();
    }
    //uniform sampler2D diffuseTex;
    //uniform sampler2D specularTex;
}

void TexturedMaterial::release()
{
}
