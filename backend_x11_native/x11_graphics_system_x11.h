#ifndef __X11_GRAPHICS_SYSTEM_X11_H__
#define __X11_GRAPHICS_SYSTEM_X11_H__

#include "x11_graphics_system.h"

class X11GraphicsSystemX11 final : public X11GraphicsSystem
{
public:
    X11GraphicsSystemX11(Display *dpy) : _dpy(dpy) {}
    virtual Canvas *createCanvas(Drawable drawable, int width, int height) override;
    virtual Icon *createIcon(int width, int height, Drawable drawable, const unsigned long *argb_data, uint32_t bg_color) override;

private:
    Display *_dpy = 0;
};

#endif
