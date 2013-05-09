#include "x11_server_widget.h"

#include "x11_application.h"
#include "x11_canvas.h"
#include "x11_global.h"

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


using namespace X11Global;


std::map<Window, X11ServerWidget*> X11ServerWidget::_wid_index;


X11ServerWidget::X11ServerWidget(Window wid, EventHandler *event_handler, const Rect &rect) : X11Widget(wid, SERVER, false, rect),
    _canvas(new X11Canvas(wid)),
    _event_handler(event_handler)
{
}

X11ServerWidget::~X11ServerWidget()
{
    _wid_index.erase(wid());

    delete _canvas;
    _canvas = 0;
    XDestroyWindow(dpy(), wid());
}

X11ServerWidget *X11ServerWidget ::find(Window wid)
{
    std::map<Window, X11ServerWidget *>::iterator it = _wid_index.find(wid);
    if (it != _wid_index.end()) {
        return it->second;
    } else
        return 0;
}

X11ServerWidget *X11ServerWidget::create(X11ServerWidget *parent, uint32 bg_color,
                                         EventHandler *event_handler, long event_mask)
{
    std::cout << "X11ServerWidget::create()\n";

    Window parent_wid = parent ? parent->wid() : X11Application::root();

    std::cout << "parent_wid: " << parent_wid << '\n';

    const unsigned int width = 100, height = 100;
//     const unsigned long fg = 0xFFFFFF;

    XSetWindowAttributes attr;
    memset(&attr, 0, sizeof(attr));

    attr.background_pixel = bg_color;
//     attr.border_pixel = fg;
    attr.event_mask = event_mask;

    Window wid = XCreateWindow(dpy(),
                               parent_wid,
                               0, 0,
                               width, height,
                               0,
                               CopyFromParent,
                               InputOutput,
                               CopyFromParent,
                               CWBackPixel /*| CWBorderPixel*/ | CWEventMask,
                               &attr);

    if (!wid)
        abort();

    assert(find(wid) == 0);

    Rect rect(0, 0, width, height);
    X11ServerWidget *widget = new X11ServerWidget(wid, event_handler, rect);

    _wid_index.insert(std::pair<Window, X11ServerWidget*>(wid, widget));

    return widget;
}

bool X11ServerWidget::handleEvent(const XEvent &ev)
{
    if (ev.type == CreateNotify && find(ev.xcreatewindow.window)) // eat create event
        return true;
    else if (ev.type == Expose && !ev.xexpose.count) {
        if (X11ServerWidget *widget = find(ev.xexpose.window)) {
            if (widget->_event_handler)
                widget->_event_handler->handleExpose();
            return true;
        } else
            return false;
    } else
        return false;
}
