#include "cairo_canvas.h"

#include "cairo_icon.h"

using namespace Cairo;

inline void rgb32_to_double(uint32 rgb, double &r, double &g, double &b)
{
    r = static_cast<double>((rgb & 0xFF0000) >> 16) / 256.f;
    g = static_cast<double>((rgb & 0x00FF00) >> 8) / 256.f;
    b = static_cast<double>(rgb & 0x0000FF) / 256.f;
}

CairoCanvas::CairoCanvas(Cairo::RefPtr<Cairo::Surface> surface) :
    _surface(surface),
    _context(Context::create(surface))
{
    _context->set_font_size(11);

//     const std::string font_family = "Monospace";
    const std::string font_family = "Sans Serif";
    _context->select_font_face(font_family, FONT_SLANT_NORMAL, FONT_WEIGHT_NORMAL);

#if 0
    FontOptions font_options;
    _context->get_font_options(font_options);

    font_options.set_hint_style(HINT_STYLE_FULL);
    font_options.set_hint_metrics(HINT_METRICS_ON);
    font_options.set_subpixel_order(SUBPIXEL_ORDER_RGB);

    _context->set_font_options(font_options);
#endif
}

void CairoCanvas::erase(const Rect &rect)
{
    fillRectangle(rect, 0);
}


void CairoCanvas::fillRectangle(const Rect &rect, uint32 color)
{
    _context->save();

    double r, g, b;
    rgb32_to_double(color, r, g, b);

    _context->set_source_rgb(r, g, b);
    _context->rectangle(rect.x, rect.y, rect.w, rect.h);
    _context->clip();
    _context->paint();

    _context->restore();
}

void CairoCanvas::drawFrame(const Rect &rect, uint32 color)
{
    _context->save();

    double r, g, b;
    rgb32_to_double(color, r, g, b);

    _context->set_source_rgb(r, g, b);
    _context->set_line_width(1);
    _context->set_line_cap(LINE_CAP_SQUARE);

    _context->move_to(rect.x + 0.5, rect.y + 0.5);
    _context->line_to(rect.x + rect.w  - 0.5, rect.y + 0.5);
    _context->line_to(rect.x + rect.w - 0.5, rect.y + rect.h - 0.5);
    _context->line_to(rect.x + 0.5, rect.y + rect.h - 0.5);
    _context->line_to(rect.x + 0.5, rect.y + 0.5);
    _context->stroke();

    _context->restore();
}

void CairoCanvas::drawText(const std::string &text, const Rect &rect, uint32 color)
{
    _context->save();

    double r, g, b;
    rgb32_to_double(color, r, g, b);
    _context->set_source_rgb(r, g, b);

    _context->rectangle(rect.x, rect.y, rect.w, rect.h);
    _context->clip();

    Cairo::FontExtents extents;
    _context->get_font_extents(extents);

    _context->move_to(rect.x + 0.5, rect.y + extents.ascent + 0.5);
    _context->show_text(text.c_str());

    _context->restore();
}

void CairoCanvas::drawIcon(Icon *icon, int x, int y)
{
    _context->save();

    CairoIcon *cairo_icon = static_cast<CairoIcon*>(icon);

    _context->set_source(cairo_icon->surface(), x, y);

    _context->rectangle(x, y, icon->width(), icon->height());
    _context->clip();

    _context->paint();

    _context->restore();
}

int CairoCanvas::maxTextHeight()
{
    Cairo::FontExtents extents;
    _context->get_font_extents(extents);

    return extents.ascent + extents.descent;
}
