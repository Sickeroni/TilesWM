#ifndef __CAIRO_CANVAS_H__
#define __CAIRO_CANVAS_H__

#include "canvas.h"

#include <cairomm/surface.h>
#include <cairomm/context.h>

class CairoCanvas : public Canvas
{
public:
    CairoCanvas(Cairo::RefPtr<Cairo::Surface> surface);
    ~CairoCanvas() {}

    virtual void erase(const Rect &rect) override final;
    virtual void fillRectangle(const Rect &rect, uint32_t color) override final;
    virtual void drawFrame(const Rect &rect, uint32_t color) override final;
    virtual void drawText(const std::string &text, const Rect &rect, uint32_t color) override final;
    virtual void drawIcon(Icon *icon, int x, int y) override final;
    virtual int maxTextHeight() override final;
    virtual void begin() override;
    virtual void end() override;

protected:
    Cairo::RefPtr<Cairo::Surface> _surface;
    Cairo::RefPtr<Cairo::Context> _context;
};


#endif // __CAIRO_CANVAS_H__
