#include "in_scene.h"
#include "OpenGL/GLStateManager.h"
#include <glm/gtc/swizzle.hpp>
#include <cmath>

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

void ProcessWindowEvent(const SDL_Event& e)
{
    switch(e.window.event)
    {
    case SDL_WINDOWEVENT_ENTER:
        fprintf(stderr, "Mouse entered our window! windowID = %d \n", e.window.windowID);
        break;
    case SDL_WINDOWEVENT_LEAVE:
        fprintf(stderr, "Mouuse left the window! windowID = %d \n", e.window.windowID);
        break;
    }
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
    case SDL_KEYDOWN:
        //if (e.key.keysym.sym == SDLK_w)
        //    keyUp = true;
        break;
    case SDL_KEYUP:
        //if (e.key.keysym.sym == SDLK_w)
        //    keyUp = false;
    default: break;
    }
}

void INScene::Update(float dt)
{
    //Using Synchronous:
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    keyUp = keys[SDL_SCANCODE_W];
    keyDown = keys[SDL_SCANCODE_S];
    keyLeft = keys[SDL_SCANCODE_A];
    keyRight = keys[SDL_SCANCODE_D];

    SDL_GetRelativeMouseState(&mouseX, &mouseY);
    if (mouseInited)
    {
        m_camera.Rotate(-0.005 * mouseY, -0.005 * mouseX);
    }
    else
    {
        //SDL_SetWindowGrab(window(), SDL_TRUE);
        SDL_SetRelativeMouseMode(SDL_TRUE);
        mouseInited = true;
    }

    static bool lastE = false;
    bool keyE = keys[SDL_SCANCODE_E];
    if (keyE && !lastE)
    {
        action();
    }
    lastE = keyE;

    int dx = 0;
    int dy = 0;
    double speed = -3;

    if (keyUp) dx += 1;
    if (keyDown) dx -= 1;
    if (keyLeft) dy += 1;
    if (keyRight) dy -= 1;

    MoveCharacter(speed*dy*dt, speed*dx*dt);

    m_counter.NextFrame(dt);
    UpdateDoor(dt);
}

void INScene::OpenDoor()
{
    if (m_doorAngVel < 0)
        m_doorAngVel = -m_doorAngVel;
}

void INScene::CloseDoor()
{
    if (m_doorAngVel > 0)
        m_doorAngVel = -m_doorAngVel;
}

void INScene::ToggleDoor()
{
    m_doorAngVel = -m_doorAngVel;
}

void INScene::MoveCharacter(float dx, float dz)
{
    vec4 forward = m_camera.getForwardDir();
    vec4 right = m_camera.getRightDir();
    vec3 temp = vec3(forward*dz + right*dx);
    vec2 tr(temp.x, temp.z);
    m_collisions.MoveCharacter(tr);
    m_camera.Move(vec3(tr.x, 0, tr.y));
}

float getAngle(glm::vec2 a, glm::vec2 b)
{
    return angle(normalize(a), normalize(b));
}

bool INScene::FacingDoor()
{
    auto rect = m_collisions.getObstacle(5);
    vec2 points[4] = { rect.getP1(), rect.getP2(), rect.getP3(), rect.getP4() };
    vec4 _forward = m_camera.getForwardDir();
    vec4 _camera = m_camera.getPosition();
    _forward = glm::swizzle(_forward, glm::X, glm::Z, glm::Y, glm::W);
    _camera = glm::swizzle(_camera, glm::X, glm::Z, glm::Y, glm::W);
    vec2 camera(_camera);
    vec2 forward(_forward);

    unsigned int max_i = 0, max_j = 0;
    float max_a = 0.0f;
    for (unsigned int i = 0; i < 4; ++i)
    {
        for (unsigned int j = i + 1; j < 4; ++j)
        {
            float a = getAngle(points[i]-camera, points[j]-camera);
            if (a > max_a)
            {
                max_i = i;
                max_j = j;
                max_a = a;
            }
        }
    }
    float AA = getAngle(forward, points[max_i]-camera);
    float BB = getAngle(forward, points[max_j]-camera);
    return equalEpsilon(BB+AA, 360.0f-max_a, 0.1f);
}

float INScene::DistanceToDoor()
{
    return m_collisions.DistanceToObstacle(5);
}

void INScene::UpdateDoor(float dt)
{
    if ((m_doorAngVel > 0 && m_doorAngle < M_PI_2) || (m_doorAngVel < 0 && m_doorAngle > 0))
    {
        m_doorAngle += dt*m_doorAngVel;
        if ( m_doorAngle < 0 )
            m_doorAngle = 0;
        else if (m_doorAngle > M_PI_2)
            m_doorAngle = M_PI_2;
        mat4 doorTransform = m_doorTransform;
        vec4 v = doorTransform * vec4(0.000004f, 0.0f, -1.08108f, 1.0f);
        doorTransform =
                glm::translate(mat4(), vec3(v))
                * glm::rotate(mat4(), degrees(m_doorAngle), vec3(0,0,1))
                * glm::translate(mat4(), -vec3(v))
                * doorTransform;
        m_sceneGraph->setNodeTransform(m_doorNode, doorTransform);
        vec2 tr = m_collisions.MoveObstacle(5, OrientedBoundingRectangle(vec2(-3.05f, 1.0f), 0.1f, 1.0f, m_doorAngle));
        m_camera.Move(vec3(tr.x, 0, tr.y));
    }
}

void INScene::displayGL()
{
    glClearColor(0.5, 0.5, 1.0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    VBGL::GLStateManager::setDefaultGLState();

    m_view = m_camera.GetViewMatrix();

    for ( unsigned int i = 0; i < m_sceneGraph->meshCount(); ++i)
    {
        Mesh& m = m_sceneGraph->getMesh(i);
        Material& material = m_sceneGraph->getMaterial(m.getMaterialIdx());

        m_model = m.getTransform();

        //Set textures, matrices
        m_material = &material;

        m_texturedEffect->bind();
        if (!material.getDiffuseTexture())
            continue;

        m.Render(m_texturedEffect->getShader());
        m_texturedEffect->release();
    }
}

void INScene::initGL()
{
    //Enable texturing
    glEnable(GL_TEXTURE_2D);

    XFileLoader xloader;
    xloader.Load("house.x");

    m_sceneGraph.reset(new SceneGraph(move(xloader.m_nodes), move(xloader.m_meshes), move(xloader.m_materials)));

    m_doorNode = m_sceneGraph->nodeByName("Door");
    m_doorTransform = m_sceneGraph->getNodeTransform(m_doorNode);
    m_doorAngle = 0;
    m_doorAngVel = -M_PI_2;

    m_texturedEffect = new TexturedMaterial(m_proj, m_view, m_model, m_material);

    float ar = static_cast<float>(width()) / height();
    m_proj = glm::perspective(60.0f, ar, 0.1f, 1000.0f);

    vector<OrientedBoundingRectangle> obstacles;
    obstacles.push_back(OrientedBoundingRectangle(vec2(-3.0f, 3.8f), 6.0f, 0.2f, 0.0f));
    obstacles.push_back(OrientedBoundingRectangle(vec2(-3.0f, -4.0f), 6.0f, 0.2f, 0.0f));
    obstacles.push_back(OrientedBoundingRectangle(vec2(2.8f, -3.8f), 0.2f, 7.6f, 0.0f));
    obstacles.push_back(OrientedBoundingRectangle(vec2(-3.0f, -3.8f), 0.2f, 4.85f, 0.0f));
    obstacles.push_back(OrientedBoundingRectangle(vec2(-3.0f, 1.95f), 0.2f, 1.85f, 0.0f));
    obstacles.push_back(OrientedBoundingRectangle(vec2(-3.05f, 1.0f), 0.1f, 1.0f, 0.0f));
    m_collisions.SetObstacles(move(obstacles));
}

void INScene::action()
{
    if (DistanceToDoor() < 1.0f && FacingDoor())
        ToggleDoor();
}

void INScene::resizeGL()
{
    glViewport(0, 0, width(), height());
}
