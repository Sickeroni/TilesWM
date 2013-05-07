#include "x11_canvas.h"

#include "x11_global.h"
#include "x11_icon.h"

#include <string.h>
#include <stdlib.h>

using namespace X11Global;

X11Canvas::X11Canvas(Drawable drawable) :
    _drawable(drawable), _gc(0)
{
    _gc = XCreateGC(dpy(), _drawable, 0, 0);
    XSetForeground(dpy(), _gc, 0xFFFFFF); //HACK
    XSetBackground(dpy(), _gc, 0x000000); //HACK
//     XSetLineAttributes(dpy(), _gc, 2, LineSolid,
//                        CapNotLast, JoinMiter);
}

X11Canvas::~X11Canvas()
{
    XFreeGC(dpy(), _gc);
}

void X11Canvas::erase(const Rect &rect)
{
    XClearArea(dpy(), _drawable, rect.x, rect.y, rect.w, rect.h, false);
}

void X11Canvas::drawFrame(const Rect &rect, unsigned long color)
{
    XSetForeground(dpy(), _gc, color);

    XDrawRectangle(dpy(), _drawable, _gc,
                   rect.x, rect.y, rect.w, rect.h);
}

void X11Canvas::drawText(const char *text, const Rect &rect,
                         unsigned long fg, unsigned long bg)
{
    XSetForeground(dpy(), _gc, fg);
    XSetBackground(dpy(), _gc, bg);

    XDrawImageString(dpy(), _drawable, _gc,
                     rect.x, rect.y + rect.h, text, strlen(text));
}

void X11Canvas::fillRectangle(const Rect &rect, unsigned long color)
{
    XSetForeground(dpy(), _gc, color);
    XFillRectangle(dpy(), _drawable, _gc, rect.x, rect.y, rect.w, rect.h);
}

void X11Canvas::drawLine(int x1, int y1, int x2, int y2, unsigned long color)
{
    XSetForeground(dpy(), _gc, color);
    XDrawLine(dpy(), _drawable, _gc, x1, y1, x2, y2);
}

void X11Canvas::drawIcon(Icon *icon, int x, int y)
{
    X11Icon *x11_icon = static_cast<X11Icon*>(icon);
    if (x11_icon->pixmap()) {
        XCopyArea(dpy(), x11_icon->pixmap(), _drawable, _gc,
                    0, 0,
                    icon->width(), icon->height(), x, y);
    }
}