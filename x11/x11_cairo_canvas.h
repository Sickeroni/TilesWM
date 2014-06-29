#ifndef __X11_CAIRO_CANVAS_H__
#define __X11_CAIRO_CANVAS_H__

#include "cairo_canvas.h"

#include <cairomm/xlib_surface.h>

class X11CairoCanvas : public CairoCanvas
{
public:
    X11CairoCanvas(Cairo::RefPtr<Cairo::XlibSurface> surface) : CairoCanvas(surface),
     _xlib_surface(surface) {}

    virtual void resize(int width, int height) override {
        _xlib_surface->set_size(width, height);
    }

private:
    Cairo::RefPtr<Cairo::XlibSurface> _xlib_surface;
};

#endif
