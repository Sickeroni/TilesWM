#include "x11_canvas.h"

#include "x11_application.h"
#include "x11_icon.h"

#include <string.h>
#include <stdlib.h>


X11Canvas::X11Canvas(Drawable drawable) :
    _drawable(drawable), _gc(0)
{
    _gc = XCreateGC(X11Application::display(), _drawable, 0, 0);
    XSetForeground(X11Application::display(), _gc, 0xFFFFFF); //HACK
    XSetBackground(X11Application::display(), _gc, 0x000000); //HACK
//     XSetLineAttributes(X11Application::display(), _gc, 2, LineSolid,
//                        CapNotLast, JoinMiter);
}

X11Canvas::~X11Canvas()
{
    XFreeGC(X11Application::display(), _gc);
}

void X11Canvas::erase(const Rect &rect)
{
    XClearArea(X11Application::display(), _drawable, rect.x, rect.y, rect.w, rect.h, false);
}

void X11Canvas::drawFrame(const Rect &rect, unsigned long color)
{
    XSetForeground(X11Application::display(), _gc, color);

    XDrawRectangle(X11Application::display(), _drawable, _gc,
                   rect.x, rect.y, rect.w, rect.h);
}

void X11Canvas::drawText(const char *text, const Rect &rect,
                         unsigned long fg, unsigned long bg)
{
    XSetForeground(X11Application::display(), _gc, fg);
    XSetBackground(X11Application::display(), _gc, bg);

    XDrawImageString(X11Application::display(), _drawable, _gc,
                     rect.x, rect.y + rect.h, text, strlen(text));
}

void X11Canvas::fillRectangle(const Rect &rect, unsigned long color)
{
    XSetForeground(X11Application::display(), _gc, color);
    XFillRectangle(X11Application::display(), _drawable, _gc, rect.x, rect.y, rect.w, rect.h);
}

void X11Canvas::drawLine(int x1, int y1, int x2, int y2, unsigned long color)
{
    XSetForeground(X11Application::display(), _gc, color);
    XDrawLine(X11Application::display(), _drawable, _gc, x1, y1, x2, y2);
}

void X11Canvas::drawIcon(Icon *icon, int x, int y)
{
    X11Icon *x11_icon = static_cast<X11Icon*>(icon);
    if (x11_icon->pixmap()) {
        XCopyArea(X11Application::display(), x11_icon->pixmap(), _drawable, _gc,
                    0, 0,
                    icon->width(), icon->height(), x, y);
    }
}