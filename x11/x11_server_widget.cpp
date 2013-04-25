#include "x11_server_widget.h"

#include "x11_application.h"
#include "x11_canvas.h"

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


std::map<Window, X11ServerWidget*> X11ServerWidget::_wid_index;


X11ServerWidget::X11ServerWidget(Window wid, EventHandler *event_handler) : X11Widget(wid, SERVER, false),
    _canvas(new X11Canvas(wid)),
    _event_handler(event_handler)
{
}

X11ServerWidget::~X11ServerWidget()
{
    _wid_index.erase(wid());

    delete _canvas;
    _canvas = 0;
    XDestroyWindow(X11Application::display(), wid());
}

X11ServerWidget *X11ServerWidget ::find(Window wid)
{
    std::map<Window, X11ServerWidget *>::iterator it = _wid_index.find(wid);
    if (it != _wid_index.end()) {
        return it->second;
    } else
        return 0;
}

X11ServerWidget *X11ServerWidget::create(X11ServerWidget *parent, EventHandler *event_handler, long event_mask)
{
    std::cout << "X11ServerWidget::create()\n";

    Window parent_wid = parent ? parent->wid() : X11Application::root();

    std::cout << "parent_wid: " << parent_wid << '\n';

    const unsigned int width = 100, height = 100;
    const unsigned long fg = 0xFFFFFF;
    const unsigned long bg = 0x999999;

    XSetWindowAttributes attr;
    memset(&attr, 0, sizeof(attr));

    attr.background_pixel = bg;
    attr.border_pixel = fg;
    attr.event_mask = event_mask;

    Window wid = XCreateWindow(X11Application::display(),
                               parent_wid,
                               0, 0,
                               width, height,
                               0,
                               CopyFromParent,
                               InputOutput,
                               CopyFromParent,
                               CWBackPixel | CWBorderPixel | CWEventMask,
                               &attr);

    if (!wid)
        abort();

    assert(find(wid) == 0);

    X11ServerWidget *widget = new X11ServerWidget(wid, event_handler);

    _wid_index.insert(std::pair<Window, X11ServerWidget*>(wid, widget));

    return widget;
}

bool X11ServerWidget::handleEvent(const XEvent &ev)
{
    if (ev.type == CreateNotify && find(ev.xcreatewindow.window)) // eat create event
        return true;
    else if (ev.type == Expose) {
        if (X11ServerWidget *widget = find(ev.xexpose.window)) {
            if (!ev.xexpose.count && widget->_event_handler)
                widget->_event_handler->handleExpose();
            return true;
        } else
            return false;
    } else
        return false;
}
