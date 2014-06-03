#pragma once

class TextMaterial;
class TextRenderer;

#include <SDL_ttf.h>
#include <VTF/Pointers>
#include "Material.h"
#include "Geometry.h"

struct FontMetrics
{
    int textWidth;
    int textHeight;
    glm::ivec2 textPos;
    int windowWidth;
    int windowHeight;
};

//For now, uses SDL!
class TextRenderer : public VTF::RefObject
{
    TTF_Font *font;
    VTF::RefPointer<TextMaterial> m_material;
    VTF::RefPointer<VBGL::Geometry> m_geometry;
    VTF::RefPointer<VBGL::Texture2D> m_texture;
    FontMetrics m_metrics;

    void RenderTextToTexture(const char* str, SDL_Color color);
public:
    TextRenderer(const char* fontFilePath, int fontSize);
    virtual ~TextRenderer();

    void getFontMetric(const char* str, int& w, int& h);
    void DrawString(const char* str, glm::vec4 color, int xPos, int yPos, int winW, int winH);
};

class TextMaterial : public VBGL::Material
{
    friend class TextRenderer;
    VTF::RefPointer<VBGL::Texture2D> m_texture;
    FontMetrics m_metrics;
public:
    TextMaterial();
    void bind();
};
