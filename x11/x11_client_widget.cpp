#include "x11_client_widget.h"

#include "x11_container_container.h"
#include "x11_client.h"
#include "x11_client_container.h"
#include "x11_server_widget.h"
#include "x11_application.h"

#include <iostream>


X11ClientWidget::X11ClientWidget(Window wid, X11Client *client) :
    X11Widget(wid, CLIENT),
    _client(client)
{
}

X11ClientWidget::~X11ClientWidget()
{
}

#if 0
bool X11ClientWidget::validate()
{
    std::cout<<"X11ClientWidget::validate()\n";
    std::cout<<"_is_destroyed: "<<_is_destroyed<<'\n';

    if (_is_destroyed)
        return false;

    Window parent_wid = 0;
    if (_client->container())
        parent_wid = static_cast<X11ClientContainer*>(_client->container())->widget()->wid();
    else
        parent_wid = X11Application::root();

    XSync(X11Application::display(), false);

    XEvent ev;


    if (XCheckTypedWindowEvent(X11Application::display(), parent_wid, DestroyNotify, &ev)) {
        std::cout<<"got destroy notify.\n";
        std::cout<<"ev.xdestroywindow.window: "<<ev.xdestroywindow.window<<'\n';
        std::cout<<"_wid: "<<wid()<<'\n';
        if (ev.xdestroywindow.window == wid())
            _is_destroyed = true;
        XPutBackEvent(X11Application::display(), &ev);
    }


    return !_is_destroyed;
}
#endif


bool X11ClientWidget::refreshMapState()
{
    XWindowAttributes attr;
    if (XGetWindowAttributes(X11Application::display(), wid(), &attr)) {
        _is_mapped = (attr.map_state != IsUnmapped);
        return true;
    } else {
        std::cerr<<"XGetWindowAttributes() failed.\n";
        return false;
    }

}

int X11ClientWidget::setRectErrorHandler(Display *display, XErrorEvent *ev)
{
    if (ev->error_code != BadWindow) {
        std::cerr<<"X11ClientWidget::setRect() caused X error - code: " <<
            static_cast<unsigned int>(ev->error_code)<<'\n';
        abort();
    } else
        return 0;
}


void X11ClientWidget::setRect(const Rect &rect)
{
    std::cout<<"---------------\nX11ClientWidget::setRect\n----------------\n";

    X11Application::self()->grabServer();

    XSync(X11Application::display(), false);

    int (*error_handler)(Display *, XErrorEvent *) =
                            XSetErrorHandler(&setRectErrorHandler);


//     if (validate())
    X11Widget::setRect(rect);

    XSync(X11Application::display(), false);

    XSetErrorHandler(error_handler);

    X11Application::self()->ungrabServer();
}