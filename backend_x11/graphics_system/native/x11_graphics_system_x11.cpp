#include "x11_graphics_system_x11.h"

#include "x11_canvas.h"
#include "x11_icon.h"

X11GraphicsSystem *createX11GraphicsSystem(Display *dpy)
{
    return new X11GraphicsSystemX11(dpy);
}

Canvas *X11GraphicsSystemX11::createCanvas(Drawable drawable, int /*width*/, int /*height*/)
{
    return new X11Canvas(_dpy, drawable);
}

Icon *X11GraphicsSystemX11::createIcon(int width, int height, Drawable parent, const unsigned long *argb_data, uint32_t bg_color)
{
    return new X11Icon(_dpy, width, height, parent, argb_data, bg_color);
}
