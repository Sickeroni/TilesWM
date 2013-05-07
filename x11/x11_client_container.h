#ifndef __X11_CLIENT_CONTAINER_H__
#define __X11_CLIENT_CONTAINER_H__

#include "client_container.h"

#include "x11_server_widget.h"


class X11ContainerContainer;


//FIXME catch expose events

class X11ClientContainer : public ClientContainer, public X11ServerWidget::EventHandler
{
public:
    X11ClientContainer(X11ContainerContainer *parent);
    virtual ~X11ClientContainer();

    virtual void setFocus();
    virtual void setRect(const Rect &rect);
    virtual void redraw();
    virtual void reparent(ContainerContainer *p);
    virtual void handleMaximizedChanged();
    virtual void handleActiveChanged() ;
    virtual int maxTextHeight();

    // X11ServerWidget::EventHandler implementaion
    virtual void handleExpose() {
        redraw();
    }

    X11ServerWidget *widget() { return _widget; }

private:
    X11ServerWidget *_widget;
};

#endif // __X11_CLIENT_CONTAINER_H__
