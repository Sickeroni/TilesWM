#include "x11_canvas.h"

#include "x11_application.h"

#include "string.h"


X11Canvas::X11Canvas(Window wid) :
    _wid(wid), _gc(0)
{
    _gc = XCreateGC(X11Application::display(), wid, 0, 0);
    XSetForeground(X11Application::display(), _gc, 0xFFFFFF); //HACK

    XSetLineAttributes(X11Application::display(), _gc, 2, LineSolid,
                       CapNotLast, JoinMiter);
}

X11Canvas::~X11Canvas()
{
}

void X11Canvas::erase(const Rect &rect)
{
    XClearArea(X11Application::display(), _wid, rect.x, rect.y, rect.w, rect.h, false);
}

void X11Canvas::drawFrame(const Rect &rect, unsigned long color)
{
    XSetForeground(X11Application::display(), _gc, color);

    XDrawRectangle(X11Application::display(), _wid, _gc,
                   rect.x, rect.y, rect.w, rect.h);
}

void X11Canvas::drawText(const char *text, const Rect &rect,
                         unsigned long fg, unsigned long bg)
{
    XSetForeground(X11Application::display(), _gc, fg);
    XSetBackground(X11Application::display(), _gc, bg);

    XDrawImageString(X11Application::display(), _wid, _gc,
                     rect.x, rect.y + rect.h, text, strlen(text));
}
