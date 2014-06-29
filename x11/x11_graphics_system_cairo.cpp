#include "x11_graphics_system_cairo.h"

#include "x11_cairo_canvas.h"
#include "cairo_icon.h"
#include "x11_global.h"

using namespace X11Global;


X11GraphicsSystem *createX11GraphicsSystem()
{
    return new X11GraphicsSystemCairo();
}

X11GraphicsSystemCairo::~X11GraphicsSystemCairo()
{
    cairo_debug_reset_static_data();
}

Canvas *X11GraphicsSystemCairo::createCanvas(Drawable drawable, int width, int height)
{
    Cairo::RefPtr<Cairo::XlibSurface> surface = Cairo::XlibSurface::create(
        dpy(),
        drawable,
        XDefaultVisual(dpy(), XDefaultScreen(dpy())),
        width,
        height);

    return new X11CairoCanvas(surface);
}

Icon *X11GraphicsSystemCairo::createIcon(int width, int height, X11Widget *parent, const unsigned long *argb_data, uint32_t bg_color)
{
    return new CairoIcon(argb_data, width, height);
}
