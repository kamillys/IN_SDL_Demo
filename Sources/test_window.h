#pragma once

#include "window_mini.h"


class TestWindow : public WindowMini
{
public:
    TestWindow(SDLApplication *app, int w, int h) : WindowMini(app, w, h) {
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
