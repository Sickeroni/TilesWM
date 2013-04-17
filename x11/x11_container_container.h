#ifndef __X11_CONTAINER_CONTAINER_H__
#define __X11_CONTAINER_CONTAINER_H__

#include "container_container.h"


class X11ServerWidget;


class X11ContainerContainer : public ContainerContainer
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

    virtual void setRect(const Rect &rect);

    X11ServerWidget *widget() { return _widget; }

protected:
    virtual ClientContainer *createClientContainer();

private:
    X11ServerWidget *_widget;
};

#endif // __X11_CONTAINER_CONTAINER_H__
