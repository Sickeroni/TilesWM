#include "x11_widget_backend.h"

#include "theme.h"
#include "widget_frontend.h"
#include "canvas.h"

X11WidgetBackend::X11WidgetBackend() :
    _widget(X11ServerWidget::create(
        0,
        0,
        this, ButtonPressMask | ExposureMask)),
    _minimized_widget(X11ServerWidget::create(
        0,
        0,
        this, ButtonPressMask | ExposureMask))
{
}

X11WidgetBackend::~X11WidgetBackend()
{
    delete _minimized_widget;
    _minimized_widget = 0;
    delete _widget;
    _widget = 0;
}

void X11WidgetBackend::setFrontend(WidgetFrontend *frontend)
{
    _frontend = frontend;
}

void X11WidgetBackend::setRect(const Rect &rect)
{
    currentWidget()->setRect(rect);
}

void X11WidgetBackend::setMapped(bool mapped)
{
    _is_mapped = mapped;
    applyMapState();
}

void X11WidgetBackend::setMinimized(bool minimized)
{
    _is_minimized = minimized;
    applyMapState();
}

void X11WidgetBackend::reparent(WidgetBackend *new_parent)
{
    X11ServerWidget *parent_widget = 0;
    if (new_parent)
        parent_widget = static_cast<X11WidgetBackend*>(new_parent)->widget();
    _widget->reparent(parent_widget);
    _minimized_widget->reparent(parent_widget);
}

void X11WidgetBackend::redraw()
{
    if (_is_mapped) //FIXME use isVisible()
        _frontend->draw(currentWidget()->canvas());
}

int X11WidgetBackend::maxTextHeight() const
{
    return const_cast<X11WidgetBackend*>(this)->currentWidget()->canvas()->maxTextHeight();
}

void X11WidgetBackend::handleButtonPress(const XButtonEvent &ev)
{
    if (_frontend)
        _frontend->handleClick(ev.x, ev.y);
}

void X11WidgetBackend::applyMapState()
{
//     printvar(_is_mapped);

    if (!_is_mapped) {
        _widget->unmap();
        _minimized_widget->unmap();
    } else if (isMinimized()) {
        _widget->unmap();
        _minimized_widget->map();
    } else {
        _minimized_widget->unmap();
        _widget->map();
    }
}
