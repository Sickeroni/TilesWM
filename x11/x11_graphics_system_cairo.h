#ifndef __X11_GRAPHICS_SYSTEM_CAIRO_H__
#define __X11_GRAPHICS_SYSTEM_CAIRO_H__

#include "x11_graphics_system.h"

class X11GraphicsSystemCairo final : public X11GraphicsSystem
{
public:
    virtual Canvas *createCanvas(Drawable drawable, int width, int height) override;
    virtual Icon *createIcon(int width, int height, X11Widget *parent, const uint32 *argb_data, uint32 bg_color) override;
};

#endif
