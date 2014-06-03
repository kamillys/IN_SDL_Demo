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
#include "OpenGL/TextRenderer.h"
#include "OpenGL/shader.h"

#include "confReader.h"

class INScene : public GLWindowBase
{
public:
    INScene(SDLApplication *app, int w, int h)
        : GLWindowBase(app, w, h),
          m_camera(glm::vec3(0, 1.6f, 0)),
          joystick(nullptr)
    {
        setTitle("SDL Window");
        keyUp = false;
        keyDown = false;
        keyLeft = false;
        keyRight = false;
        mouseInited = false;
        prevAction = false;
        keyAction = false;
    }
    virtual ~INScene()
    {
        if (joystick)
            SDL_JoystickClose(joystick);
    }

protected:

    bool keyUp;
    bool keyDown;
    bool keyLeft;
    bool keyRight;
    double mouseX, mouseY;
    bool mouseInited;
    bool prevAction;
    bool keyAction;
    bool doAction;

    bool joystickPresent = false;
    SDL_Joystick *joystick;

    enum class ActionKey {
            MoveUp,
            MoveDown,
            MoveLeft,
            MoveRight,
            Action
    };
    std::map<ActionKey, SDL_Scancode> keyMapping;
    ConfigReader confReader;
    void loadKeyMapping();

    void action();

    virtual void resizeGL();
    virtual void initGL();
    virtual void displayGL();
    virtual void Update(float dt);
    virtual void ProcessEvent(const SDL_Event& event);

    void DrawString();

    //Starts the door opening animation if the door isn't fully open
    void OpenDoor();
    //Starts the door closing animation if the door isn't fully closed
    void CloseDoor();
    //Toggles between the door opening and closing animation
    void ToggleDoor();
    //Moves the character forward by dx and right by dz in relation to the current camera orientation
    void MoveCharacter(float dx, float dz);
    //Checks if the camera is facing the door
    bool FacingDoor();
    //Returns the distance from the character to the door
    float DistanceToDoor();

    void UpdateDoor(float dt);

    VTF::RefPointer<TexturedMaterial> m_texturedEffect;
    std::unique_ptr<mini::SceneGraph> m_sceneGraph;

    mini::Material* m_material;
    VTF::RefPointer<TextRenderer> m_textRenderer;
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
