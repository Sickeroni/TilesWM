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
    virtual void drawFrame(const Rect &rect, unsigned long color);
    virtual void drawText(const char *text, const Rect &rect,
                          unsigned long fg, unsigned long bg);
    virtual void drawIcon(Icon *icon, int x, int y);

    void fillRectangle(const Rect &rect, unsigned long color);
    void drawLine(int x1, int y1, int x2, int y2, unsigned long color);

private:
    Drawable _drawable;
    GC _gc;
};


#endif // __X11_CANVAS_H__
