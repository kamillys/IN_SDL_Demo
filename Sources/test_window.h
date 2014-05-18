#pragma once

#include "window_base.h"


class TestWindow : public GLWindowBase
{
public:
    TestWindow(SDLApplication *app, int w, int h) : GLWindowBase(app, w, h) {
        setTitle("Sample");
    }
protected:
    virtual void resizeGL();
    virtual void initGL();
    virtual void displayGL();
};

class TestWindow2  : public TestWindow
{
public:
    TestWindow2(SDLApplication *app) : TestWindow(app, 800, 600){}
    virtual ~TestWindow2() {}
};
