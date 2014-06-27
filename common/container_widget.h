#ifndef __CONTAINER_WIDGET_H__
#define __CONTAINER_WIDGET_H__

#include "widget_base.h"

class ContainerBase;
class Client;


class ContainerWidget : public WidgetBase
{
public:
    virtual void reparent(WidgetBase *parent) = 0;
    virtual void redraw() = 0;
    virtual int maxTextHeight() const = 0;
    virtual void setMinimized(bool minimized) = 0;
    // intended as a rendering hint -
    // if set, only child needs to be rendered
    virtual void setMappedChild(WidgetBase *child) {}

protected:
    ContainerWidget(ContainerBase *container) : WidgetBase(CONTAINER),
        _container(container) {}

    ContainerBase * const _container = 0;
};

#endif
