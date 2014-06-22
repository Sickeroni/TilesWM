#ifndef __CAIRO_ICON_H__
#define __CAIRO_ICON_H__

#include "icon.h"
#include "common.h"

#include <cairomm/surface.h>

class CairoIcon final : public Icon
{
public:
    CairoIcon(const unsigned long *argb32_data, int width, int height);
    CairoIcon(const CairoIcon &other);
    CairoIcon &operator=(const CairoIcon &other);
    ~CairoIcon() {}

    const Cairo::RefPtr<Cairo::Surface> &surface() { return _surface; }

private:
    Cairo::RefPtr<Cairo::Surface> _surface;
};

#endif // __CAIRO_ICON_H__
