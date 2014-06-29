#ifndef __X11_WIDGET__BACKEND__
#define __X11_WIDGET__BACKEND__

#include "widget_backend.h"

#include "x11_server_widget.h"

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
    virtual void setFrontend(Widget *frontend) override;

    // X11ServerWidget::EventHandler implementaion
    virtual void handleExpose() override {
        redraw();
    }
    virtual void handleButtonPress(const XButtonEvent &ev) override;

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
    Widget *_frontend = 0;
};

#endif
