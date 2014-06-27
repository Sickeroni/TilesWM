#ifndef __X11_CONTAINER_WIDGET__
#define __X11_CONTAINER_WIDGET__

#include "container_widget.h"

#include "x11_server_widget.h"

class X11ContainerWidget final : public ContainerWidget, public X11ServerWidget::EventHandler
{
public:
    X11ContainerWidget(ContainerBase *container);
    ~X11ContainerWidget();

    virtual void setRect(const Rect &rect) override;
    virtual void setMapped(bool mapped) override;
    virtual void reparent(WidgetBase *parent) override;
    virtual void redraw() override;
    virtual int maxTextHeight() const override;
    virtual void setMinimized(bool minimized) override;

    // X11ServerWidget::EventHandler implementaion
    virtual void handleExpose() override {
        redraw();
    }
    virtual void handleButtonPress(const XButtonEvent &ev) override;

    bool isMinimized() { return _is_minimized; }
    void reparent(X11ContainerWidget *p);
    X11ServerWidget *x11Widget() { return _widget; }

private:
    X11ServerWidget *currentWidget() {
        return isMinimized() ? _minimized_widget : _widget;
    }
    void applyMapState();

    X11ServerWidget *_widget = 0, *_minimized_widget = 0;
    bool _is_minimized = false;
    bool _is_mapped = false;
};

#endif
