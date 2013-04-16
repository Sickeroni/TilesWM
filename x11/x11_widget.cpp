#include "x11_widget.h"

// #include "x11_container_container.h"
#include "x11_client_widget.h"
// #include "x11_client.h"
#include "x11_application.h"

#include <iostream>
#include <stdlib.h>


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

    //FIXME add ro _from_wid
    abort();
}


X11Widget::~X11Widget()
{
    abort();
    //FIXME remove from _from_wid
}

void X11Widget::setRect(const Rect &rect)
{
    XMoveResizeWindow(X11Application::display(), _wid, rect.x, rect.y, rect.w, rect.h);
}

X11Widget *X11Widget::find(Window wid)
{
    abort();
    //FIXME
// d  std::map<Window, X11Widget*>::iterator it = _from_wid.find(wid);
//       if (it != _from_wid.end()) {
// 	
//       }
}

void X11Widget::createNotify(const XCreateWindowEvent &ev)
{
    Window wid = ev.window;
    std::cout << "X11Widget::CreateNotify(): " << wid << " parent: " << ev.parent << '\n';
    if (find(wid)) {
        // it's a server widget
        std::cout << "it's a server widget.\n";
    } else {
        X11ClientWidget::newClientWidget(wid);
    }
}

void X11Widget::destroyNotify(const XDestroyWindowEvent &ev)
{
    Window wid = ev.window;
    std::cout << "X11Widget::DestroyNotify(): " << wid << '\n';
    if (X11Widget *widget = find(wid)) {
        if (widget->type() == X11Widget::CLIENT) {
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
    //FIXME
    abort();
}
