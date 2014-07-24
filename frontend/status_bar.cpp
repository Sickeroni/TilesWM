#include "status_bar.h"

#include "application.h"
#include "canvas.h"
#include "backend.h"

StatusBar::StatusBar() : ChildWidget(OTHER)
{
    _backend = Application::self()->backend()->createWidgetBackend();
    _backend->setFrontend(this);
}

StatusBar::~StatusBar()
{
    delete _backend;
    _backend = 0;
}

void StatusBar::draw(Canvas *canvas)
{
    Rect rect = _rect;
    rect.setPos(0, 0);

    canvas->begin();

    canvas->erase(_rect);
    if (!_text.empty())
        canvas->drawText(_text, rect, 0x00FF00);

    canvas->end();
}

void StatusBar::setText(const std::string &text)
{
    _text = text;
    redraw();
}
