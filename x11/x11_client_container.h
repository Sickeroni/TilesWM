#ifndef __X11_CLIENT_CONTAINER_H__
#define __X11_CLIENT_CONTAINER_H__

#include "client_container.h"

#include "x11_server_widget.h"
#include "x11_client.h"

#include <vector>

class X11ContainerContainer;
class X11Client;


//FIXME catch expose events

class X11ClientContainer : public ClientContainer, public X11ServerWidget::EventHandler
{
public:
    X11ClientContainer(X11ContainerContainer *parent);
    virtual ~X11ClientContainer();

//     virtual void setFocus();
    virtual void setRect(const Rect &rect);
    virtual void redraw();
    virtual void reparent(ContainerContainer *p);
    virtual void handleMaximizedChanged();
    virtual void handleActiveChanged() ;
    virtual int maxTextHeight();
    virtual int numElements() { return _children.size(); }
    virtual Client *child(int index) { return _children[index]; }
    virtual int indexOfChild(Client *child);

    // X11ServerWidget::EventHandler implementaion
    virtual void handleExpose() {
        redraw();
    }
    virtual void handleButtonPress(const XButtonEvent &ev);

    X11ServerWidget *widget() { return _widget; }

private:
    X11ServerWidget *currentWidget() {
        return isMinimized() ? _minimized_widget : _widget;
    }

    std::vector<X11Client*> _children;
    X11ServerWidget *_widget, *_minimized_widget;
};

#endif // __X11_CLIENT_CONTAINER_H__
