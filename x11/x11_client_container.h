#ifndef __X11_CLIENT_CONTAINER_H__
#define __X11_CLIENT_CONTAINER_H__

#include "client_container.h"


class X11ServerWidget;
class X11ContainerContainer;


//FIXME catch expose events

class X11ClientContainer : public ClientContainer
{
public:
    X11ClientContainer(X11ContainerContainer *parent);
    virtual ~X11ClientContainer();

    virtual void setRect(const Rect &rect);
    virtual void redraw();

    X11ServerWidget *widget() { return _widget; }

private:
    X11ServerWidget *_widget;
};

#endif // __X11_CLIENT_CONTAINER_H__
