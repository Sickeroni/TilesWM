#include "status_bar.h"

#include "application.h"
#include "canvas.h"
#include "backend.h"

StatusBar::StatusBar() : ChildWidget(OTHER)
{
}

StatusBar::~StatusBar()
{
}

void StatusBar::draw(Canvas *canvas)
{
    Rect r = rect();
    r.setPos(0, 0);

    canvas->begin();

    canvas->erase(r);
    if (!_text.empty())
        canvas->drawText(_text, r, 0x00FF00);

    canvas->end();
}

void StatusBar::setText(const std::string &text)
{
    _text = text;
    redraw();
}
