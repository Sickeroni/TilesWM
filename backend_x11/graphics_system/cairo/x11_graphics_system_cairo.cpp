#include "x11_graphics_system_cairo.h"

#include "x11_cairo_canvas.h"
#include "cairo_icon.h"

X11GraphicsSystem *createX11GraphicsSystem(Display *dpy)
{
    return new X11GraphicsSystemCairo(dpy);
}

X11GraphicsSystemCairo::~X11GraphicsSystemCairo()
{
    cairo_debug_reset_static_data();
}

Canvas *X11GraphicsSystemCairo::createCanvas(Drawable drawable, int width, int height)
{
    Cairo::RefPtr<Cairo::XlibSurface> surface = Cairo::XlibSurface::create(
        _dpy,
        drawable,
        XDefaultVisual(_dpy, XDefaultScreen(_dpy)),
        width,
        height);

    return new X11CairoCanvas(surface);
}

Icon *X11GraphicsSystemCairo::createIcon(int width, int height, Drawable /*parent*/, const unsigned long *argb_data, uint32_t /*bg_color*/)
{
    return new CairoIcon(argb_data, width, height);
}

Icon *X11GraphicsSystemCairo::loadImage(std::string filename)
{
    try {
        Cairo::RefPtr<Cairo::ImageSurface> surface = Cairo::ImageSurface::create_from_png(filename);
        return new CairoIcon(surface);
    }
    catch(Cairo::logic_error error) {
        debug<<"error loading"<<filename;
    }

    return 0;
}
