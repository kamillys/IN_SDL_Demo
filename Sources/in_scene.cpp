#include "in_scene.h"
#include "OpenGL/GLStateManager.h"

#include "mini_xfileLoader.h"

using namespace std;
using namespace mini;
using namespace glm;

void JoyButton(const SDL_Event &e)
{
    std::cerr << "Button: " << e.jbutton.which
              << " " << e.jbutton.button << " " << e.jbutton.state
              << "\n";
}

void INScene::ProcessEvent(const SDL_Event &e)
{
    switch(e.type)
    {
        case SDL_JOYAXISMOTION:
        case SDL_JOYBALLMOTION:
        case SDL_JOYHATMOTION:
        case SDL_JOYBUTTONUP:
        case SDL_JOYBUTTONDOWN:
        case SDL_JOYDEVICEADDED:
        case SDL_JOYDEVICEREMOVED:
        std::cerr << "JOYEVENT" << std::endl;
    default: break;
    }
}

void INScene::Update(float dt)
{
    T += dt;
}

void INScene::displayGL()
{
    glClearColor(0.2, 0.2, 0.2, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    VBGL::GLStateManager::setDefaultGLState();
    VBGL::GLStateManager::disableClipping();
    //VBGL::GLStateManager::disableDepthTest();

    //XMStoreFloat4x4(&m_proj, XMMatrixPerspectiveFovLH(XMConvertToRadians(60.0f), ar, 0.01f, 100.0f));
    float ar = static_cast<float>(width()) / height();
    m_proj = glm::perspective(glm::radians(60.0f), ar, 0.01f, 100.0f);

    //m_view = glm::translate(glm::mat4(), glm::vec3(0, 0, 0)) /* glm::rotate(mat4(), 90.0f, vec3(1, 0, 0))*/;
    m_view = glm::lookAt(-10.0f*(sinf(T)*0.5f+0.5f)*vec3(1, 1, -1), vec3(), vec3(0, 0, 1)) * glm::scale(mat4(), vec3(1/20.0, 1/20.0, 1/20.0))
            * glm::scale(mat4(), vec3(1, 1, 0.05))
            ;

    glm::mat4 mvp = m_proj * m_view * m_model;

    //std::cerr << (mvp * vec4(0,0,0,1)) << " " << (mvp * vec4(1,0,0,1)) << "\n";
    //std::cerr << "=================================================================\n";

    //m_context->RSSetState(m_rsState.get());
    //XMFLOAT4X4 mtx[2];
    //XMStoreFloat4x4(&mtx[0], m_camera.GetViewMatrix());
    //m_cbView->Update(m_context, mtx[0]);
    //m_texturedEffect->Begin(m_context);
    for ( unsigned int i = 0; i < m_sceneGraph->meshCount(); ++i)
    {
            Mesh& m = m_sceneGraph->getMesh(i);
            Material& material = m_sceneGraph->getMaterial(m.getMaterialIdx());

            //Set textures, matrices
            m_material = &material;

            m_texturedEffect->bind();
            //if (!material.getDiffuseTexture())
            //        continue;
            //ID3D11ShaderResourceView* srv[2] = { material.getDiffuseTexture().get(), material.getSpecularTexture().get() };
            //m_context->PSSetShaderResources(0, 2, srv);
            //mtx[0] = m.getTransform();
            //XMMATRIX modelit = XMLoadFloat4x4(&mtx[0]);
            //XMVECTOR det;
            //modelit = XMMatrixTranspose(XMMatrixInverse(&det, modelit));
            //XMStoreFloat4x4(&mtx[1], modelit);
            //m_cbMaterial->Update(m_context, material.getMaterialData());
            //m_cbModel->Update(m_context, mtx);
            //m_context->IASetInputLayout(m_layout.get());

            m.Render(m_texturedEffect->getShader());
            m_texturedEffect->release();
    }
    //RenderText();
}

void INScene::initGL()
{
    glEnable(GL_TEXTURE_2D);

    XFileLoader xloader;
    xloader.Load("house.x");

    m_sceneGraph.reset(new SceneGraph(move(xloader.m_nodes), move(xloader.m_meshes), move(xloader.m_materials)));

    //m_doorNode = m_sceneGraph->nodeByName("Door");
    //m_doorTransform = m_sceneGraph->getNodeTransform(m_doorNode);
    m_doorAngle = 0;
    m_doorAngVel = -M_PI_2;

    m_texturedEffect = new TexturedMaterial(m_proj, m_view, m_model, m_material);

    //XMStoreFloat4x4(&m_proj, XMMatrixPerspectiveFovLH(XMConvertToRadians(60.0f), ar, 0.01f, 100.0f));
    float ar = static_cast<float>(width()) / height();
    //m_proj = glm::perspective(glm::radians(60.0f), ar, 0.01f, 100.0f);
}

void INScene::resizeGL()
{
    glViewport(0, 0, width(), height());
}
