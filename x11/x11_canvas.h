#ifndef __X11_CANVAS_H__
#define __X11_CANVAS_H__


#include "canvas.h"

#include <X11/Xlib.h>


class X11Canvas : public Canvas
{
public:
    X11Canvas(Drawable drawable);
    ~X11Canvas();

    virtual void erase(const Rect &rect);
    virtual void fillRectangle(const Rect &rect, uint32 color);
    virtual void drawFrame(const Rect &rect, uint32 color);
    virtual void drawText(const std::string &text, const Rect &rect, uint32 color);
    virtual void drawIcon(Icon *icon, int x, int y);

    int maxTextHeight();
    void drawLine(int x1, int y1, int x2, int y2, uint32 color);

    GC gc() { return _gc; }

private:
    Drawable _drawable;
    GC _gc;
    XFontStruct *_font_info;
};


#endif // __X11_CANVAS_H__
