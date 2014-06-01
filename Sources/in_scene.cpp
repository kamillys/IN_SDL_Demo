#include "in_scene.h"
#include "OpenGL/GLStateManager.h"
#include <glm/gtc/swizzle.hpp>
#include <cmath>
#include "util.h"

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
    SDL_Scancode code;
    switch(e.type)
    {
    case SDL_JOYAXISMOTION:
        //SDL_Log("Joystick %d axis %d value: %d\n", e.jaxis.which, e.jaxis.axis, e.jaxis.value);
        break;
    case SDL_JOYBALLMOTION:
        break;
    case SDL_JOYHATMOTION:
        SDL_Log("Joystick %d hat %d value:", e.jhat.which, e.jhat.hat);
        if (e.jhat.value == SDL_HAT_CENTERED) SDL_Log(" centered");
        if (e.jhat.value & SDL_HAT_UP) SDL_Log(" up");
        if (e.jhat.value & SDL_HAT_RIGHT) SDL_Log(" right");
        if (e.jhat.value & SDL_HAT_DOWN) SDL_Log(" down");
        if (e.jhat.value & SDL_HAT_LEFT) SDL_Log(" left");
        SDL_Log("\n");
        break;
    case SDL_JOYBUTTONDOWN:
        SDL_Log("Joystick %d button %d down\n", e.jbutton.which, e.jbutton.button);
        break;
    case SDL_JOYBUTTONUP:
        SDL_Log("Joystick %d button %d up\n", e.jbutton.which, e.jbutton.button);
        break;
    case SDL_JOYDEVICEADDED:
        if (joystickPresent) break;
        joystick = SDL_JoystickOpen(e.jdevice.which);
        joystickPresent = joystick != nullptr;
        std::cerr << "Joystick added!\n";
        break;
    case SDL_JOYDEVICEREMOVED:
        std::cerr << "Some joystick removed!" << std::endl;
        if (SDL_JoystickInstanceID(joystick) == e.jdevice.which)
        {
            std::cerr << "Disconnected joystick!" << std::endl;
            joystickPresent = false;
            SDL_JoystickClose(joystick);
            joystick = nullptr;
        }
        break;
    case SDL_KEYDOWN:
        code = e.key.keysym.scancode;
        //Asynchronous
        /*
        if (code == keyMapping[ActionKey::MoveUp]) keyUp = true;
        if (code == keyMapping[ActionKey::MoveDown]) keyDown = true;
        if (code == keyMapping[ActionKey::MoveLeft]) keyLeft = true;
        if (code == keyMapping[ActionKey::MoveRight]) keyRight = true;
        */
        doAction |= (code == keyMapping[ActionKey::Action]);
        break;
    case SDL_KEYUP:
        code = e.key.keysym.scancode;
        //Asynchronous
        /*
        if (code == keyMapping[ActionKey::MoveUp]) keyUp = false;
        if (code == keyMapping[ActionKey::MoveDown]) keyDown = false;
        if (code == keyMapping[ActionKey::MoveLeft]) keyLeft = false;
        if (code == keyMapping[ActionKey::MoveRight]) keyRight = false;
        */
        break;
    case SDL_MOUSEMOTION:
        /*
        mouseX = -0.005 * e.motion.xrel;
        mouseY = -0.005 * e.motion.yrel;
        m_camera.Rotate(mouseY, mouseX);
        */
    default: break;
    }
}

void INScene::loadKeyMapping()
{
    keyMapping[ActionKey::MoveUp] = confReader.getKeyMap("MoveUp", "W");
    keyMapping[ActionKey::MoveDown] = confReader.getKeyMap("MoveDown", "S");
    keyMapping[ActionKey::MoveLeft] = confReader.getKeyMap("MoveLeft", "A");
    keyMapping[ActionKey::MoveRight] = confReader.getKeyMap("MoveRight", "D");
    keyMapping[ActionKey::Action] = confReader.getKeyMap("Action", "E");

    //padDriver.setJoyMap("MoveUp", confReader.getString("JMoveUp", "NegY"));
    //padDriver.setJoyMap("MoveDown", confReader.getString("JMoveDown", "PosY"));
    //padDriver.setJoyMap("MoveLeft", confReader.getString("JMoveLeft", "NegX"));
    //padDriver.setJoyMap("MoveRight", confReader.getString("JMoveRight", "PosX"));
    //padDriver.setJoyMap("Action", confReader.getString("JAction", "Button1"));
    //padDriver.setJoyMap("MoveUp", "dpadYpos");
}

Sint16 clamp(Sint16 v, Sint16 m, Sint16 M)
{
    return v < m ? m : v > M ? M : v;
}

void INScene::Update(float dt)
{
    double speed = -3;

    //Using Synchronous:
    //Begin synch

    if (!joystickPresent)
    {
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    keyUp = keys[keyMapping[ActionKey::MoveUp]];
    keyDown = keys[keyMapping[ActionKey::MoveDown]];
    keyLeft = keys[keyMapping[ActionKey::MoveLeft]];
    keyRight = keys[keyMapping[ActionKey::MoveRight]];
    keyAction = keys[keyMapping[ActionKey::Action]];
    } else
    {
        Sint16 Xaxis = SDL_JoystickGetAxis(joystick, 0);
        Xaxis = -clamp(Xaxis, -10000, 10000);
        float XSpeed = Xaxis / 10000.0;
        Sint16 Yaxis = SDL_JoystickGetAxis(joystick, 1);
        Yaxis = -clamp(Yaxis, -10000, 10000);
        float YSpeed = Yaxis / 10000.0;
        keyAction = SDL_JoystickGetButton(joystick, 0);
        MoveCharacter(XSpeed * speed * dt, YSpeed * speed * dt);
    }
    doAction = keyAction && !prevAction;
    prevAction = keyAction;

    int imouseX, imouseY;
    SDL_GetRelativeMouseState(&imouseX, &imouseY);
    mouseX = -0.005 * imouseX;
    mouseY = -0.005 * imouseY;

    //End Synch

    int dx = 0;
    int dy = 0;

    if (!joystickPresent)
    {
        if (keyUp) dx += 1;
        if (keyDown) dx -= 1;
        if (keyLeft) dy += 1;
        if (keyRight) dy -= 1;
        MoveCharacter(speed*dy*dt, speed*dx*dt);
    }

    if (doAction)
        action();
    doAction = false;

    //Comment in case of async
    m_camera.Rotate(mouseY, mouseX);


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
    /*
    joystickPresent = SDL_NumJoysticks() > 0;
    if (joystickPresent)
    {
        joystick = SDL_JoystickOpen(0);
        if (joystick == NULL) {
            DebugLog("SDL_JoystickOpen failed: %s", SDL_GetError());
        } else
        {
            char guid[64];
            SDL_JoystickGetGUIDString(SDL_JoystickGetGUID(joystick),
                                      guid, sizeof (guid));
            SDL_Log("       axes: %d\n", SDL_JoystickNumAxes(joystick));
            SDL_Log("      balls: %d\n", SDL_JoystickNumBalls(joystick));
            SDL_Log("       hats: %d\n", SDL_JoystickNumHats(joystick));
            SDL_Log("    buttons: %d\n", SDL_JoystickNumButtons(joystick));
            SDL_Log("instance id: %d\n", SDL_JoystickInstanceID(joystick));
            SDL_Log("       guid: %s\n", guid);
            SDL_JoystickClose(joystick);
        }
    }*/
    SDL_SetRelativeMouseMode(SDL_TRUE);
    //SDL_SetWindowGrab(window(), SDL_TRUE);

    loadKeyMapping();
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
