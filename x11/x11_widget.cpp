#include "x11_widget.h"

#include "x11_container_container.h"
#include "x11_client.h"
#include "x11_server_widget.h"
#include "x11_client_widget.h"
#include "x11_application.h"

#include <iostream>
#include <stdlib.h>
#include <assert.h>


X11Widget::X11Widget(Window wid, Type type, bool is_mapped, const Rect &rect) :
    _rect(rect),
    _is_destroyed(false),
    _is_mapped(is_mapped),
    _wid(wid),
    _type(type)
{
}


X11Widget::~X11Widget()
{
}

#if 0
bool X11Widget::validate()
{
    std::cout<<"X11Widget::validate()\n";

    XEvent ev;

    if (_is_destroyed)
        return false;
#if 0
    if (XCheckTypedWindowEvent(X11Application::display(), _wid, DestroyNotify, &ev)) {
        std::cout<<"got destroy notify.\n";
        std::cout<<"ev.xdestroywindow.window: "<<ev.xdestroywindow.window<<'\n';
        std::cout<<"_wid: "<<_wid<<'\n';
        if (ev.xdestroywindow.window == _wid)
            _is_destroyed = true;
        XPutBackEvent(X11Application::display(), &ev);
        return !_is_destroyed;
    }
#else
    if (XCheckTypedEvent(X11Application::display(), DestroyNotify, &ev)) {
        std::cout<<"got destroy notify.\n";
        std::cout<<"ev.xdestroywindow.window: "<<ev.xdestroywindow.window<<'\n';
        std::cout<<"_wid: "<<_wid<<'\n';
        if (ev.xdestroywindow.window == _wid)
            _is_destroyed = true;
        XPutBackEvent(X11Application::display(), &ev);
        return !_is_destroyed;
    }
#endif

    return true;
}
#endif

void X11Widget::map()
{
//     assert(!_is_mapped);
    if (!_is_mapped) {
        XMapWindow(X11Application::display(), _wid);
        _is_mapped = true;
    }
}

void X11Widget::unmap()
{
//     assert(_is_mapped);
    if (_is_mapped) {
        XUnmapWindow(X11Application::display(), _wid);
        _is_mapped = false;
    }
}

void X11Widget::reparent(X11ServerWidget *new_parent, int x, int y)
{
    Window new_parent_wid = new_parent ? new_parent->wid() : X11Application::root();
    XReparentWindow(X11Application::display(), _wid, new_parent_wid, x, y);
}

void X11Widget::move(int x, int y)
{
    XMoveWindow(X11Application::display(), _wid, x, y);
}

void X11Widget::setRect(const Rect &rect)
{
    std::cout<<"X11Widget::setRect() - _wid: "<<_wid<<'\n';
    assert(rect.w && rect.h);
    _rect.set(rect);
    XMoveResizeWindow(X11Application::display(), _wid, rect.x, rect.y, rect.w, rect.h);
}

bool X11Widget::handleEvent(const XEvent &ev)
{
//     std::cout<<"X11Widget::handleEvent(): "<<X11Application::x11EventToString(ev.type)<<'\n';

    bool handled = false;

    handled = X11ServerWidget::handleEvent(ev);

    if (!handled)
        handled = X11Client::handleEvent(ev);

    return handled;
}
