#ifndef __X11_CONTAINER_CONTAINER_H__
#define __X11_CONTAINER_CONTAINER_H__

#include "container_container.h"

class X11ContainerContainer : public ContainerContainer
{
#if 0
    ContainerContainer *createContainerContainer()
    {
        ContainerContainer *c = new ContainerContainer(this);
        X11ServerWidget *w = new X11ServerWidget(_widget);
    };

    X11ServerWidget *_widget;
#endif

public:
    X11ContainerContainer();

    void setRect(const Rect &rect);
};

#endif // __X11_CONTAINER_CONTAINER_H__
