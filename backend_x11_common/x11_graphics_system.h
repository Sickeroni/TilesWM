#ifndef __X11_GRAPHICS_SYSTEM_H__
#define __X11_GRAPHICS_SYSTEM_H__

#include "common.h"

#include <X11/Xlib.h>

class Icon;
class Canvas;

class X11GraphicsSystem
{
public:
    virtual ~X11GraphicsSystem() {}

    virtual Canvas *createCanvas(Drawable drawable, int width, int height) = 0;
    virtual Icon *createIcon(int width, int height, Drawable parent, const unsigned long *argb_data, uint32_t bg_color) = 0;
};

#endif
