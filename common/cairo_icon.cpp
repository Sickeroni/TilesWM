#include "cairo_icon.h"

using namespace Cairo;

CairoIcon::CairoIcon(const unsigned long *argb32_data, int width, int height) : Icon(width, height)
{
    RefPtr<ImageSurface> surface = ImageSurface::create(FORMAT_ARGB32, width, height);

    int stride = surface->get_stride();
    unsigned char *data = surface->get_data();

    for(int y = 0; y < height; y++) {
        uint32 *scanline = reinterpret_cast<uint32*>(data + (y * stride));

        for (int x = 0; x < width; x++)
            scanline[x] = argb32_data[(y * width) + x];
    }

    surface->mark_dirty();

    _surface = surface;
}
