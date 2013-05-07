#include "x11_global.h"

#include "x11_application.h"


Display *X11Global::dpy()
{
    return X11Application::dpy();
}
