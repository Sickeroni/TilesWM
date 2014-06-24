#ifndef __X11_GRAPHICS_SYSTEM_CAIRO_H__
#define __X11_GRAPHICS_SYSTEM_CAIRO_H__

#include "x11_graphics_system.h"

class X11GraphicsSystemCairo final : public X11GraphicsSystem
{
public:
    ~X11GraphicsSystemCairo();

    virtual Canvas *createCanvas(Drawable drawable, int width, int height) override;
    virtual Icon *createIcon(int width, int height, X11Widget *parent, const unsigned long *argb_data, uint32_t bg_color) override;
};

#endif
