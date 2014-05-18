#pragma once

#include "window_base.h"


class INScene : public GLWindowBase
{
public:
    INScene(SDLApplication *app, int w, int h) : GLWindowBase(app, w, h) {
        setTitle("Sample");
    }
    virtual ~INScene(){}
protected:
    virtual void resizeGL(){}
    virtual void initGL(){}
    virtual void displayGL(){}
    virtual void Update(float dt){}
    virtual void ProcessEvent(const SDL_Event& event){}
};
