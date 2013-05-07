#include "x11_canvas.h"

#include "x11_global.h"
#include "x11_icon.h"

#include <string.h>
#include <stdlib.h>
#include <assert.h>


using namespace X11Global;


X11Canvas::X11Canvas(Drawable drawable) :
    _drawable(drawable), _gc(0), _font_info(0)
{
    _gc = XCreateGC(dpy(), _drawable, 0, 0);
    XSetForeground(dpy(), _gc, 0xFFFFFF); //HACK
    XSetBackground(dpy(), _gc, 0x000000); //HACK
//     XSetLineAttributes(dpy(), _gc, 2, LineSolid,
//                        CapNotLast, JoinMiter);

    _font_info = XQueryFont(dpy(), XGContextFromGC(_gc));
}

X11Canvas::~X11Canvas()
{
    if (_font_info)
        XFreeFontInfo(0, _font_info, 0);
    _font_info = 0;
    XFreeGC(dpy(), _gc);
    _gc = 0;
}

int X11Canvas::maxTextHeight()
{
    assert(_font_info);

    return _font_info->max_bounds.ascent + _font_info->max_bounds.descent;
}

void X11Canvas::erase(const Rect &rect)
{
    XClearArea(dpy(), _drawable, rect.x, rect.y, rect.w, rect.h, false);
}

void X11Canvas::drawFrame(const Rect &rect, uint32 color)
{
    XSetForeground(dpy(), _gc, color);

    XDrawRectangle(dpy(), _drawable, _gc,
                   rect.x, rect.y, rect.w, rect.h);
}

//FIXME use std::string as parameter
void X11Canvas::drawText(const char *text, const Rect &rect,
                         uint32 fg, uint32 bg)
{
    assert(_font_info);

    XSetForeground(dpy(), _gc, fg);
    XSetBackground(dpy(), _gc, bg);

    XTextItem text_item = {
        const_cast<char*>(text),
        strlen(text),
        0,
        None
    };

    XDrawText(dpy(), _drawable, _gc, rect.x, rect.y + _font_info->max_bounds.ascent,
              &text_item,
              1);
}

void X11Canvas::fillRectangle(const Rect &rect, uint32 color)
{
    XSetForeground(dpy(), _gc, color);
    XFillRectangle(dpy(), _drawable, _gc, rect.x, rect.y, rect.w, rect.h);
}

void X11Canvas::drawLine(int x1, int y1, int x2, int y2, uint32 color)
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
