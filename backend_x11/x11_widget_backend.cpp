#include "x11_widget_backend.h"

#include "x11_global.h"
#include "x11_application.h"
#include "widget_frontend.h"
#include "canvas.h"

#include <X11/Xcursor/Xcursor.h>
#include <X11/cursorfont.h>

using namespace X11Global;

X11WidgetBackend *X11WidgetBackend::_mouse_grabber = 0;

X11WidgetBackend::X11WidgetBackend() :
    _widget(X11ServerWidget::create(
        0,
        0,
        this,
        ButtonPressMask | ExposureMask | SubstructureNotifyMask | SubstructureRedirectMask)),
    _minimized_widget(X11ServerWidget::create(
        0,
        0,
        this,
        ButtonPressMask | ExposureMask))
{
}

X11WidgetBackend::~X11WidgetBackend()
{
    assert(this != _mouse_grabber);
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
    if (_is_mapped && _frontend) //FIXME use isVisible()
        _frontend->draw(currentWidget()->canvas());
}

int X11WidgetBackend::maxTextHeight() const
{
    return const_cast<X11WidgetBackend*>(this)->currentWidget()->canvas()->maxTextHeight();
}

void X11WidgetBackend::grabMouse(CursorType cursor_type)
{
    assert(!_mouse_grabber);
    _mouse_grabber = this;

    unsigned int cursor_shape = XC_left_ptr;

    switch (cursor_type) {
        case CURSOR_MOVE:
            cursor_shape = XC_fleur;
            break;
        case CURSOR_RESIZE_BOTTOM_RIGHT:
            cursor_shape = XC_bottom_right_corner;
            break;
        case CURSOR_DEFAULT:
        default:
            break;
    }

    Cursor cursor = X11Application::self()->getCursor(cursor_shape);

    XGrabPointer(dpy(),currentWidget()->wid(), true,
                 PointerMotionMask | ButtonReleaseMask,
                 GrabModeAsync, GrabModeAsync, None, cursor, CurrentTime);
}

void X11WidgetBackend::releaseMouse()
{
    assert(_mouse_grabber == this);
    _mouse_grabber = 0;
    XUngrabPointer(dpy(), CurrentTime);
}

void X11WidgetBackend::handleMouseEvent(const XButtonEvent &ev)
{
    if (_frontend) {
        switch (ev.type) {
            case ButtonPress:
                _frontend->handleButtonPress(ev.x_root, ev.y_root, ev.button);
                break;
            case ButtonRelease:
                _frontend->handleButtonRelease(ev.button);
                break;
            case MotionNotify:
                _frontend->handleMouseMove(ev.x_root, ev.y_root);
                break;
        }
    }
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

void X11WidgetBackend::raise()
{
    XRaiseWindow(dpy(), currentWidget()->wid());
}
