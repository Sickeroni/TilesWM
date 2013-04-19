#ifndef __X11_CANVAS_H__
#define __X11_CANVAS_H__


#include "canvas.h"

#include <X11/Xlib.h>


class X11Canvas : public Canvas
{
public:
    X11Canvas(Window wid);
    ~X11Canvas();

    void erase(const Rect &rect);
    virtual void drawFrame(const Rect &rect, unsigned long color);
    virtual void drawText(const char *text, const Rect &rect,
                          unsigned long fg, unsigned long bg);

private:
    Window _wid;
    GC _gc;
};


#endif // __X11_CANVAS_H__
