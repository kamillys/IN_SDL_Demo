#pragma once

#include "OpenGL/myGL.h"
#include <glm/glm.hpp>
#include <memory>

#include "window_base.h"
#include "mini_fpsCounter.h"
#include "mini_camera.h"
#include "mini_collision.h"
#include "mini_sceneGraph.h"
#include "OpenGL/TexturedMaterial.h"

#include "OpenGL/shader.h"

class INScene : public GLWindowBase
{
public:
    INScene(SDLApplication *app, int w, int h)
        : GLWindowBase(app, w, h),
          m_camera(glm::vec3(0, 1.6f, 0))
    {
        setTitle("Sample");
    }
    virtual ~INScene(){}

    float T = 0;

protected:
    virtual void resizeGL();
    virtual void initGL();
    virtual void displayGL();
    virtual void Update(float dt);
    virtual void ProcessEvent(const SDL_Event& event);


    //IDirectInput8* di;
    //IDirectInputDevice8* pMouse;
    //IDirectInputDevice8* pKeyboard;

    //std::unique_ptr<ConstantBuffer<DirectX::XMFLOAT4X4>> m_cbProj;
    //std::unique_ptr<ConstantBuffer<DirectX::XMFLOAT4X4>> m_cbView;
    //std::unique_ptr<ConstantBuffer<DirectX::XMFLOAT4X4, 2> > m_cbModel;
    //std::unique_ptr<ConstantBuffer<mini::Material::MaterialData>> m_cbMaterial;

    VTF::RefPointer<TexturedMaterial> m_texturedEffect;
    //std::unique_ptr<mini::TexturedEffect> m_texturedEffect;
    std::unique_ptr<mini::SceneGraph> m_sceneGraph;

    mini::Material* m_material;
    //mini::dx_ptr<ID3D11InputLayout> m_layout;
    //mini::dx_ptr<ID3D11RasterizerState> m_rsState;
    //mini::dx_ptr<ID3D11SamplerState> m_sampler;
    //mini::dx_ptr<IFW1Factory> m_fontFactory;
    //mini::dx_ptr<IFW1FontWrapper> m_font;
    mini::Camera m_camera;
    glm::mat4 m_proj;
    glm::mat4 m_view;
    glm::mat4 m_model;
    mini::FPSCounter m_counter;

    unsigned int m_doorNode;
    float m_doorAngle;
    float m_doorAngVel;
    glm::mat4 m_doorTransform;
    mini::CollisionEngine m_collisions;



};
