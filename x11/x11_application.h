#ifndef __X11_APPLICATION_H__
#define __X11_APPLICATION_H__

#include <X11/Xlib.h>

#if 1

class X11ContainerContainer;

class X11Application
{
public:
    static X11Application *self() { return _self; }
    static Display *display()  { return self()->_display; }
    static Window root()  { return self()->_root; }
    static X11ContainerContainer *activeRootContainer() {
        return self()->_activeRootContainer;
    }

    X11Application();

    bool init();
    void shutdown();
    void eventLoop();

private:
    static X11Application *_self;

    Display *_display;
    Window _root;
    X11ContainerContainer *_activeRootContainer;
};

#endif

#endif // __X11_APPLICATION_H__
