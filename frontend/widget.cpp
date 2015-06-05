#include "widget.h"

#include "widget_backend.h"
#include "workspace.h"
#include "theme.h"
#include "child_widget.h"
#include "common.h"
#include "backend.h"

Widget::Widget(Type type, WidgetBackend *backend) :
    _backend(backend),
    _type(type)
{
    if (_backend)
        _owns_backend = false;
    else {
        _owns_backend = true;
        _backend = Application::self()->backend()->createWidgetBackend();
    }

    _backend->setFrontend(this);
}

Widget::~Widget() {
    if (_owns_backend) {
        delete _backend;
        _backend = 0;
    } else {
        _backend->setFrontend(0);
        _backend->setMapped(false);
        _backend->reparent(0);
        _backend = 0;
    }
}

void Widget::setRect(const Rect &rect)
{
    _backend->setRect(rect);
    _rect.set(rect);
}

void Widget::setMapped(bool mapped)
{
    _backend->setMapped(mapped);
}

void Widget::setMinimized(bool minimized)
{
    _is_minimized = minimized;
    _backend->setMinimized(minimized);
}

void Widget::redraw()
{
    _backend->redraw();
}

int Widget::maxTextHeight() const
{
    return _backend->maxTextHeight();
}

void Widget::reparent(Widget *parent) {
    setMapped(false);

    _rect.setPos(0, 0);

    _backend->reparent(parent ? parent->_backend : 0);
}

Workspace *Widget::toWorkspace()
{
    if (_type == WORKSPACE)
        return static_cast<Workspace*>(this);
    else
        return 0;
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
