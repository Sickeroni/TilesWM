#include "x11_graphics_system_x11.h"

#include "x11_canvas.h"
#include "x11_icon.h"

X11GraphicsSystem *createX11GraphicsSystem()
{
    return new X11GraphicsSystemX11();
}

Canvas *X11GraphicsSystemX11::createCanvas(Drawable drawable, int width, int height)
{
    return new X11Canvas(drawable);
}

Icon *X11GraphicsSystemX11::createIcon(int width, int height, X11Widget *parent, const unsigned long *argb_data, uint32_t bg_color)
{
    return new X11Icon(width, height, parent, argb_data, bg_color);
}
