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
//    std::cerr << "\n===============================\n";
//    std::cerr << m_cbProj;
//    std::cerr << "\n===============================\n";
//    std::cerr << m_cbView;
//    std::cerr << "\n===============================\n";
//    std::cerr << m_cbModel;
//    std::cerr << "\n===============================\n";
//    std::cerr << mvp;
//    std::cerr << "\n===============================";
//    std::cerr << "\n===============================";
//    std::cerr << "\n===============================\n";
    shader->setUniform("UMVP", mvp);
    shader->setUniform("diffuseColor", m_cbMaterial->getMaterialData().diffuseColor);
    shader->setUniform("specularColor", m_cbMaterial->getMaterialData().specularColor);

    VBGL::Texture2D* diffTex = m_cbMaterial->getDiffuseTexture();
    VBGL::Texture2D* specTex = m_cbMaterial->getSpecularTexture();
    if (diffTex)
    {
        glActiveTexture( GL_TEXTURE0 );
        diffTex->bind();
    }
    if (specTex)
    {
        glActiveTexture( GL_TEXTURE1 );
        specTex->bind();
    }
    //uniform sampler2D diffuseTex;
    //uniform sampler2D specularTex;
}

void TexturedMaterial::release()
{
}
