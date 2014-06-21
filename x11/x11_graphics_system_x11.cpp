#include "x11_graphics_system_x11.h"

#include "x11_canvas.h"
#include "x11_icon.h"

Canvas *X11GraphicsSystemX11::createCanvas(Drawable drawable)
{
    return new X11Canvas(drawable);
}

Icon *X11GraphicsSystemX11::createIcon(int width, int height, X11Widget *parent, const uint32 *argb_data, uint32 bg_color)
{
    return new X11Icon(width, height, parent, argb_data, bg_color);
}
