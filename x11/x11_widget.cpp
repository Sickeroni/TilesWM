#include "x11_widget.h"

#include "x11_container_container.h"
#include "x11_client.h"
#include "x11_server_widget.h"
#include "x11_client_widget.h"
#include "x11_application.h"

#include <iostream>
#include <stdlib.h>
#include <assert.h>


std::map<Window, X11Widget*> X11Widget::_wid_index;


X11Widget::X11Widget(Window wid, Type type) :
    _is_destroyed(false),
    _wid(wid),
    _type(type),
    _is_mapped(false)
{
    XWindowAttributes attr;
    if (!XGetWindowAttributes(X11Application::display(), _wid, &attr))
        abort();

//     _rect.set(attr.x, attr.y, attr.width, attr.height);
    _is_mapped = (attr.map_state != IsUnmapped);

    assert(find(wid) == 0);

    _wid_index.insert(std::pair<Window, X11Widget*>(_wid, this));
}


X11Widget::~X11Widget()
{
    _wid_index.erase(_wid);
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
    XMapWindow(X11Application::display(), _wid);
    _is_mapped = true;
}

void X11Widget::unmap()
{
    XUnmapWindow(X11Application::display(), _wid);
    _is_mapped = false;
}

void X11Widget::reparent(X11ServerWidget *new_parent)
{
    Window new_parent_wid = new_parent ? new_parent->wid() : X11Application::root();
    XReparentWindow(X11Application::display(), _wid, new_parent_wid, 0, 0);
}

void X11Widget::move(int x, int y)
{
    XMoveWindow(X11Application::display(), _wid, x, y);
}

void X11Widget::setRect(const Rect &rect)
{
    std::cout<<"X11Widget::setRect() - _wid: "<<_wid<<'\n';
    XMoveResizeWindow(X11Application::display(), _wid, rect.x, rect.y, rect.w, rect.h);
}

X11Widget *X11Widget::find(Window wid)
{
    std::map<Window, X11Widget*>::iterator it = _wid_index.find(wid);
    if (it != _wid_index.end()) {
        return it->second;
    } else
        return 0;
}

void X11Widget::createNotify(const XCreateWindowEvent &ev)
{
#if 1
    Window wid = ev.window;
    std::cout << "X11Widget::CreateNotify(): " << wid << " parent: " << ev.parent << '\n';
    if (find(wid)) {
        // it's a server widget
        std::cout << "it's a server widget.\n";
#if 0
        //FIXME debug-message
        X11Widget *w = X11Application::activeRootContainer()->widget();
        std::cout<<"widget: "<<find(wid)<<'\t';
        std::cout<<"active root container widget: "<< w <<'\n';
        if (X11Application::activeRootContainer()->activeChild()) {
            std::cout<<"has child.\n";
//             X11Application::activeRootContainer()->activeChild()
        }
#endif
    } else {
        X11Client::handleCreate(wid);
    }
#endif
}

void X11Widget::destroyNotify(const XDestroyWindowEvent &ev)
{
    Window wid = ev.window;

    std::cout << "X11Widget::destroyNotify() - wid: "<<wid<<'\n';

    if (X11Widget *widget = find(wid)) {
        widget->_is_destroyed = true;

        if (widget->type() == X11Widget::CLIENT)
            X11Client::handleDestroy(widget);
        else {
            // error: server widget was destroyed (by another process ?) before deleting the associated X11Widget
            std::cerr << "error: server widget was destroyed (by another process ?) before deleting the associated X11Widget\n";
            abort();
        }
    } else
        std::cout << "no widget for wid " << wid <<'\n';
}

void X11Widget::mapNotify(const XMapEvent &ev)
{
    std::cout << "void X11Widget::mapNotify(const XMapEvent &ev)\n";

    if (X11Widget *widget = find(ev.window)) {
        widget->_is_mapped = true;
        widget->onMapStateChanged();
    } else
        std::cout << "no widget for wid " << ev.window;
}

void X11Widget::unmapNotify(const XUnmapEvent &ev)
{
    std::cout << "void X11Widget::unmapNotify(const XUnmapEvent &ev)\n";

    if (X11Widget *widget = find(ev.window)) {
        widget->_is_mapped = false;
        widget->onMapStateChanged();
    } else
        std::cout << "no widget for wid " << ev.window << '\n';
}
