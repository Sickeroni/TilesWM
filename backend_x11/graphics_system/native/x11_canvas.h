#ifndef __X11_CANVAS_H__
#define __X11_CANVAS_H__


#include "canvas.h"

#include <X11/Xlib.h>


class X11Canvas final : public Canvas
{
public:
    X11Canvas(Display *display, Drawable drawable);
    ~X11Canvas();

    virtual void erase(const Rect &rect) override;
    virtual void fillRectangle(const Rect &rect, uint32_t color) override;
    virtual void drawFrame(const Rect &rect, uint32_t color) override;
    virtual void drawText(const std::string &text, const Rect &rect, uint32_t color) override;
    virtual void drawIcon(Icon *icon, int x, int y) override;
    virtual int maxTextHeight() override;

    void drawLine(int x1, int y1, int x2, int y2, uint32_t color);

    GC gc() { return _gc; }
    Display *dpy() { return _dpy; }

private:
    Drawable _drawable = None;
    GC _gc = None;
    XFontStruct *_font_info = 0;
    Display *_dpy = 0;
};


#endif // __X11_CANVAS_H__
