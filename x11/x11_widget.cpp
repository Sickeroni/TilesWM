#include "x11_widget.h"

#include "x11_container_container.h"
#include "x11_client_widget.h"
#include "x11_server_widget.h"
// #include "x11_client.h"
#include "x11_application.h"

#include <iostream>
#include <stdlib.h>
#include <assert.h>


std::map<Window, X11Widget*> X11Widget::_wid_index;


X11Widget::X11Widget(Window wid, Type type) :
    _wid(wid),
    _type(type),
    _is_mapped(false)
{
    XWindowAttributes attr;
    if (!XGetWindowAttributes(X11Application::display(), _wid, &attr))
        abort();

//     _rect.set(attr.x, attr.y, attr.width, attr.height);
    _is_mapped = (attr.map_state == IsViewable);

    assert(find(wid) == 0);

    _wid_index.insert(std::pair<Window, X11Widget*>(_wid, this));
}


X11Widget::~X11Widget()
{
    _wid_index.erase(_wid);
}

bool X11Widget::validate()
{
    XEvent ev;

    if (_is_destroyed)
        return false;
    else if (XCheckTypedWindowEvent(X11Application::display(), _wid, DestroyNotify, &ev)) {
        if (ev.xdestroywindow.window == _wid)
            _is_destroyed = true;
        XPutBackEvent(X11Application::display(), &ev);
        return _is_destroyed;
    } else
        return true;
}

void X11Widget::reparent(X11ServerWidget *new_parent)
{
    Window new_parent_wid = new_parent ? new_parent->wid() : X11Application::root();
    XReparentWindow(X11Application::display(), _wid, new_parent_wid, 0, 0);
}

void X11Widget::setRect(const Rect &rect)
{
    XMoveResizeWindow(X11Application::display(), _wid, rect.x, rect.y, rect.w, rect.h);
}

void X11Widget::initClientWidgets()
{
    Window unused = 0;
    Window *children = 0;
    unsigned int num_children = 0;

    XQueryTree(X11Application::display(), X11Application::root(),
               &unused, &unused, &children, &num_children);

    for (unsigned int i = 0; i < num_children; i++) { //FIXME - what if a window gets destroyed inbetween ?
                                                      //maybe query for destroy events at each step ?
        if (!find(children[i]))
            X11ClientWidget::newClientWidget(children[i]);
    }

    XFree(children);
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

        //FIXME debug-message
        X11Widget *w = X11Application::activeRootContainer()->widget();
        std::cout<<"widget: "<<find(wid)<<'\t';
        std::cout<<"active root container widget: "<< w <<'\n';
        if (X11Application::activeRootContainer()->activeChild()) {
            std::cout<<"has child.\n";
//             X11Application::activeRootContainer()->activeChild()
        }

    } else {
        X11ClientWidget::newClientWidget(wid);
    }
#endif
}

void X11Widget::destroyNotify(const XDestroyWindowEvent &ev)
{
    Window wid = ev.window;
    std::cout << "X11Widget::DestroyNotify(): " << wid << '\n';
    if (X11Widget *widget = find(wid)) {
        if (widget->type() == X11Widget::CLIENT) {
            widget->_is_destroyed = true;
            delete widget;
            widget = 0;
        }
        else {
            // error: server widget was destroyed (by another process ?) before deleting the associated X11Widget
            std::cerr << "error: server widget was destroyed (by another process ?) before deleting the associated X11Widget\n";
            abort();
        }
    } else
        std::cout << "no widget for wid " << wid;
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
