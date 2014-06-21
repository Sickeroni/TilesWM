#ifndef __X11_GRAPHICS_SYSTEM_H__
#define __X11_GRAPHICS_SYSTEM_H__

#include "common.h"

#include <X11/Xlib.h>

class Icon;
class Canvas;
class X11Widget;

class X11GraphicsSystem
{
public:
    virtual ~X11GraphicsSystem() {}

    virtual Canvas *createCanvas(Drawable drawable) = 0;
    virtual Icon *createIcon(int width, int height, X11Widget *parent, const uint32 *argb_data, uint32 bg_color) = 0;
};

#endif
