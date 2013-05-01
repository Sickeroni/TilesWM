#include "x11_client_widget.h"

#include "x11_container_container.h"
#include "x11_client.h"
#include "x11_client_container.h"
#include "x11_server_widget.h"
#include "x11_application.h"

#include <iostream>


X11ClientWidget::X11ClientWidget(Window wid, X11Client *client, bool is_mapped, const Rect &rect) :
    X11Widget(wid, CLIENT, is_mapped, rect)
//     ,
//     _client(client)
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
