#include "x11_canvas.h"

#include "x11_global.h"
#include "x11_icon.h"

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
    debug;
    // while not mentionend in in http://www.x.org/releases/X11R7.7/doc/libX11/libX11/libX11.html#XFreeFontInfo,
    // 'actual_count' also refers to the number of font structures passed in 'free_info'
    if (_font_info)
        XFreeFontInfo(0, _font_info, 1);
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
//     XClearArea(dpy(), _drawable, rect.x, rect.y, rect.w, rect.h, false);
    fillRectangle(rect, 0);
}

void X11Canvas::drawFrame(const Rect &rect, uint32_t color)
{
    XSetForeground(dpy(), _gc, color);
    // the width and height parameters of XDrawRectangle()
    // are actually offsets, so we have to subtract 1
    XDrawRectangle(dpy(), _drawable, _gc,
                   rect.x, rect.y, rect.w - 1, rect.h - 1);
}

void X11Canvas::drawText(const std::string &text, const Rect &rect, uint32_t color)
{
    assert(_font_info);

    XRectangle x_rect = {
        static_cast<short>(rect.x),
        static_cast<short>(rect.y),
        static_cast<unsigned short>(rect.w),
        static_cast<unsigned short>(rect.h)
    };

    XSetClipRectangles(dpy(), _gc,
                       0, 0,
                       &x_rect,
                       1,
                       Unsorted);

    XSetForeground(dpy(), _gc, color);

    XTextItem text_item = {
        const_cast<char*>(text.c_str()),
        static_cast<int>(text.length()),
        0,
        None
    };

    XDrawText(dpy(), _drawable, _gc, rect.x, rect.y + _font_info->max_bounds.ascent,
              &text_item,
              1);

    XSetClipMask(dpy(), _gc, None);
}

void X11Canvas::fillRectangle(const Rect &rect, uint32_t color)
{
    XSetForeground(dpy(), _gc, color);
    XFillRectangle(dpy(), _drawable, _gc, rect.x, rect.y, rect.w, rect.h);
}

void X11Canvas::drawLine(int x1, int y1, int x2, int y2, uint32_t color)
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
