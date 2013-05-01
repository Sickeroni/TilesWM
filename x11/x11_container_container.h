#ifndef __X11_CONTAINER_CONTAINER_H__
#define __X11_CONTAINER_CONTAINER_H__

#include "container_container.h"

#include "x11_server_widget.h"


class X11ContainerContainer : public ContainerContainer, public X11ServerWidget::EventHandler
{
#if 0
    ContainerContainer *createContainerContainer()
    {
        ContainerContainer *c = new ContainerContainer(this);
        X11ServerWidget *w = new X11ServerWidget(_widget);
    };

#endif

public:
    X11ContainerContainer(X11ContainerContainer *parent);
    virtual ~X11ContainerContainer();

    // X11ServerWidget::EventHandler implementaion
    virtual void handleExpose() {
        redraw();
    }

    virtual void redraw();
    virtual void setRect(const Rect &rect);

    virtual void reparent(ContainerContainer *p);

    X11ServerWidget *widget() { return _widget; }

protected:
    virtual ContainerContainer *createContainerContainer();
    virtual ClientContainer *createClientContainer();

private:
    X11ServerWidget *_widget;
};

#endif // __X11_CONTAINER_CONTAINER_H__
