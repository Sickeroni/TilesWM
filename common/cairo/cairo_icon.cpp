#include "cairo_icon.h"

#include <cairomm/context.h>

using namespace Cairo;

CairoIcon::CairoIcon(const unsigned long *argb32_data, int width, int height) : Icon(width, height)
{
    RefPtr<ImageSurface> surface = ImageSurface::create(FORMAT_ARGB32, width, height);

    int stride = surface->get_stride();
    unsigned char *data = surface->get_data();

    for(int y = 0; y < height; y++) {
        uint32_t *scanline = reinterpret_cast<uint32_t*>(data + (y * stride));

        for (int x = 0; x < width; x++)
            scanline[x] = argb32_data[(y * width) + x];
    }

    surface->mark_dirty();

    _surface = surface;
}

CairoIcon::CairoIcon(Cairo::RefPtr<Cairo::ImageSurface> surface) :
    Icon(surface->get_width(), surface->get_height())
{
    _surface = surface;
}

Icon *CairoIcon::scale(int width, int height)
{
    RefPtr<ImageSurface> surface = ImageSurface::create(FORMAT_ARGB32, width, height);

    RefPtr<Context> context = Context::create(surface);

    context->scale((double)width / (double)_width, (double)height / (double)_height);
    context->set_source(_surface, 0, 0);

    context->paint();

    return new CairoIcon(surface);
}
