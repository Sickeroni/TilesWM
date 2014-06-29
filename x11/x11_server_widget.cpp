#include "x11_server_widget.h"

#include "x11_graphics_system.h"
#include "x11_application.h"
#include "x11_global.h"
#include "common.h"

#include "canvas.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>


using namespace X11Global;


std::map<Window, X11ServerWidget*> X11ServerWidget::_wid_index;


X11ServerWidget::X11ServerWidget(Window wid, EventHandler *event_handler, const Rect &rect) : X11Widget(wid, SERVER, false, rect),
    _canvas(X11Application::graphicsSystem()->createCanvas(wid, rect.w, rect.h)),
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

X11ServerWidget *X11ServerWidget::create(X11ServerWidget *parent, uint32_t bg_color,
                                         EventHandler *event_handler, long event_mask)
{
    debug;

    Window parent_wid = parent ? parent->wid() : X11Application::root();

    printvar(parent_wid);

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
//                                CWBackPixel |
//                                    CWBorderPixel |
                                   CWEventMask,
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
    else if (ev.type == ButtonPress) {
        X11ServerWidget *widget = 0;
        if (ev.xbutton.window != X11Application::root())
            widget = find(ev.xbutton.window);
        else if (ev.xbutton.subwindow)
            widget = find(ev.xbutton.subwindow);
        if (widget) {
            if (widget->_event_handler)
                widget->_event_handler->handleButtonPress(ev.xbutton);
            return true;
        } else
            return false;
    } else if (ev.type == Expose && !ev.xexpose.count) {
        if (X11ServerWidget *widget = find(ev.xexpose.window)) {
            if (widget->_event_handler)
                widget->_event_handler->handleExpose();
            return true;
        } else
            return false;
    } else
        return false;
}

void X11ServerWidget::setRect(const Rect &rect)
{
    X11Widget::setRect(rect);
    delete _canvas;
    _canvas = X11Application::graphicsSystem()->createCanvas(wid(), rect.w, rect.h);
}

void X11ServerWidget::resize(int w, int h)
{
    X11Widget::resize(w, h);
    delete _canvas;
    _canvas = X11Application::graphicsSystem()->createCanvas(wid(), w, h);
}
