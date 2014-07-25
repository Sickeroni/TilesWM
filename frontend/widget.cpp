#include "widget.h"

#include "widget_backend.h"
#include "workspace.h"
#include "theme.h"
#include "child_widget.h"
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

Workspace *Widget::toWorkspace()
{
    if (_type == WORKSPACE)
        return static_cast<Workspace*>(this);
    else
        return 0;
}

void Widget::draw(Canvas *canvas)
{
    Theme::drawWidget(this, canvas);
}

void Widget::globalToLocal(int &x, int &y)
{
    if (ChildWidget *child_widget = toChildWidget()) {
        if (child_widget->parent())
            child_widget->parent()->globalToLocal(x, y);
    }
    x -= _rect.x;
    y -= _rect.y;
}
