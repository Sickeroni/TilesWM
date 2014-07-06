#include "widget.h"

#include "widget_backend.h"
#include "workspace.h"
#include "client.h"
#include "frontend_theme.h"
#include "common.h"

void Widget::setRect(const Rect &rect)
{
    _backend->setRect(rect);
    _rect.set(rect);
}

void Widget::setMapped(bool mapped)
{
    _backend->setMapped(mapped);
}

void Widget::redraw()
{
    _backend->redraw();
}

int Widget::maxTextHeight() const
{
    return _backend->maxTextHeight();
}

Client *Widget::toClient()
{
    if (_type == CLIENT)
        return static_cast<Client*>(this);
    else
        return 0;
}

Workspace *Widget::toWorkspace()
{
    if (_type == WORKSPACE)
        return static_cast<Workspace*>(this);
    else
        return 0;
}

// Container *Widget::toContainer()
// {
//     abort();
// }

void Widget::draw(Canvas *canvas)
{
    Theme::drawWidget(this, canvas);
}
