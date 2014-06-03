#include "TextRenderer.h"
#include "texture2d.h"
#include <stdexcept>
#include "Material.h"


TextMaterial::TextMaterial()
{
    shader = new VBGL::ShaderProgram;

    shader->addShader(
                "#version 130\n"
                "attribute vec3 in_vertices;"
                "attribute vec2 in_UVs;"
                "out vec2 uv;"
                "uniform int UTextW;"
                "uniform int UTextH;"
                "uniform int UWinW;"
                "uniform int UWinH;"
                "uniform vec2 UTextPos;"
                "void main(){"
                "vec2 pos = in_vertices.xy;"
                "vec2 win = 0.5 * vec2(UWinW, UWinH);"
                "vec2 text = vec2(UTextW, UTextH);"
                "vec2 textwin = text / win;"
                "vec2 tpos = vec2(1, -1) * UTextPos / win;"
                "pos = pos * textwin;"
                "pos = pos + vec2(-1,1) + vec2(0, -textwin.y) + tpos;"
                "gl_Position = vec4(pos, -1.0, 1.0);"
                "uv = in_UVs;"
                "}"
                , VBGL::ShaderProgram::VertexShader);
    shader->addShader(
                "#version 130\n"
                "uniform sampler2D UTexture;"
                "in vec2 uv;"
                "out vec4 fcolor;"
                "void main(){"
                "fcolor.rgba = texture2D(UTexture, uv).rgba;"
                "}"
                , VBGL::ShaderProgram::FragmentShader);

    shader->build();
}

void TextMaterial::bind(){
    shader->bind();
    shader->setUniform("UTextW", m_metrics.textWidth);
    shader->setUniform("UTextH", m_metrics.textHeight);
    shader->setUniform("UWinW", m_metrics.windowWidth);
    shader->setUniform("UWinH", m_metrics.windowHeight);
    shader->setUniform("UTextPos", glm::vec2(m_metrics.textPos));
    if (m_texture)
    {
        shader->setUniform("UTexture", 0);
        glActiveTexture( GL_TEXTURE0 );
        m_texture->bind();
    }
}

TextRenderer::TextRenderer(const char* fontFilePath, int fontSize)
    : m_texture(nullptr), font(nullptr)
{
    font = TTF_OpenFont(fontFilePath, fontSize);
    if (font == nullptr)
        THROW_EXCEPTION("Cannot load font file!");

    m_material = new TextMaterial();
    m_geometry = new VBGL::Geometry;
    m_geometry->verticesAppend(glm::vec3(0,0,0));
    m_geometry->verticesAppend(glm::vec3(1,0,0));
    m_geometry->verticesAppend(glm::vec3(1,1,0));
    m_geometry->verticesAppend(glm::vec3(0,1,0));
    m_geometry->UVsAppend(glm::vec2(0,1));
    m_geometry->UVsAppend(glm::vec2(1,1));
    m_geometry->UVsAppend(glm::vec2(1,0));
    m_geometry->UVsAppend(glm::vec2(0,0));

    m_geometry->indicesAppend(0);
    m_geometry->indicesAppend(1);
    m_geometry->indicesAppend(2);
    m_geometry->indicesAppend(0);
    m_geometry->indicesAppend(2);
    m_geometry->indicesAppend(3);

    m_geometry->indicesAppend(3);
    m_geometry->indicesAppend(2);
    m_geometry->indicesAppend(0);
    m_geometry->indicesAppend(2);
    m_geometry->indicesAppend(1);
    m_geometry->indicesAppend(0);
}

TextRenderer::~TextRenderer()
{
    if (font)
        TTF_CloseFont(font);
}

void TextRenderer::getFontMetric(const char* str, int& w, int& h)
{
    TTF_SizeText(font, str, &w, &h);
}

void TextRenderer::RenderTextToTexture(const char *str, SDL_Color color)
{
    //Render the message to an SDL_Surface
    TTF_SizeText(font, str, &m_metrics.textWidth, &m_metrics.textHeight);
    SDL_Surface *surf = TTF_RenderText_Blended(font, str, color);
    //TODO: Reuse texture. In future releases.
    m_texture = VBGL::Texture2D::loadTexture(surf);
    //Clean up unneeded stuff
    SDL_FreeSurface(surf);
}

void TextRenderer::DrawString(const char* str, glm::vec4 color, int xPos, int yPos, int winW, int winH)
{
    SDL_Color c;
    c.a = 0xff * color.a;
    c.r = 0xff * color.r;
    c.g = 0xff * color.g;
    c.b = 0xff * color.b;
    RenderTextToTexture(str, c);
    m_metrics.windowWidth = winW;
    m_metrics.windowHeight = winH;
    m_metrics.textPos = glm::ivec2(xPos, yPos);
    m_material->m_texture = m_texture;
    m_material->m_metrics = m_metrics;

    //Draw it
    m_material->bind();
    m_geometry->indicedDraw(m_material->getShader());
    m_material->release();

    m_texture = nullptr;
}
