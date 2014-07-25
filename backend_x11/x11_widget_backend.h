#ifndef __X11_WIDGET__BACKEND__
#define __X11_WIDGET__BACKEND__

#include "widget_backend.h"

#include "x11_server_widget.h"

class WidgetFrontend;

class X11WidgetBackend final : public WidgetBackend, public X11ServerWidget::EventHandler
{
public:
    X11WidgetBackend();
    ~X11WidgetBackend();

    virtual void setRect(const Rect &rect) override;
    virtual void setMapped(bool mapped) override;
    virtual void reparent(WidgetBackend *parent) override;
    virtual void redraw() override;
    virtual int maxTextHeight() const override;
    virtual void setMinimized(bool minimized) override;
    virtual void setFrontend(WidgetFrontend *frontend) override;
    virtual void grabMouse(CursorType cursor) override;
    virtual void releaseMouse() override;
    virtual void raise() override;

    // X11ServerWidget::EventHandler implementaion
    virtual void handleExpose() override {
        redraw();
    }
    virtual void handleMouseEvent(const XButtonEvent &ev) override;

    bool isMinimized() { return _is_minimized; }
//     void reparent(X11WidgetBackend *new_parent);
    X11ServerWidget *widget() { return _widget; }

private:
    X11ServerWidget *currentWidget() {
        return isMinimized() ? _minimized_widget : _widget;
    }
    void applyMapState();

    X11ServerWidget *_widget = 0, *_minimized_widget = 0;
    bool _is_minimized = false;
    bool _is_mapped = false;
    WidgetFrontend *_frontend = 0;

    static X11WidgetBackend *_mouse_grabber;
};

#endif
