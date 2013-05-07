#ifndef __CANVAS_H__
#define __CANVAS_H__

#include "rect.h"
#include "types.h"

#include <string>


class Icon;


class Canvas
{
public:
    virtual void erase(const Rect &rect) = 0;
    virtual void drawFrame(const Rect &rect, uint32 color) = 0;
    virtual void drawText(const std::string &text, const Rect &rect,
                          uint32 fg, uint32 bg) = 0;
    virtual void drawIcon(Icon *icon, int x, int y) = 0;
};


#endif // __CANVAS_H__
