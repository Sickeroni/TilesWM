#ifndef __CANVAS_H__
#define __CANVAS_H__

#include "rect.h"

class Icon;

class Canvas
{
public:
    virtual void erase(const Rect &rect) = 0;
    virtual void drawFrame(const Rect &rect, unsigned long color) = 0;
    virtual void drawText(const char *text, const Rect &rect,
                          unsigned long fg, unsigned long bg) = 0;
    virtual void drawIcon(Icon *icon, int x, int y) = 0;
};

#endif // __CANVAS_H__
