#ifndef __CANVAS_H__
#define __CANVAS_H__

#include "rect.h"


class Canvas
{
public:
    virtual void drawText(const char *text, const Rect &rect) = 0;
};

#endif // __CANVAS_H__
