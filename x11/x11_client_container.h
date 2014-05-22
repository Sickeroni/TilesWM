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
    X11ClientContainer();
    virtual ~X11ClientContainer();

//     virtual void setFocus();
    virtual void setMapped(bool mapped);
    virtual void setRect(const Rect &rect);
    virtual void handleMaximizedChanged();
    virtual void handleActiveChanged() ;
    virtual int numElements() { return _children.size(); }
    virtual bool isEmpty() { return _children.empty(); }
    virtual Client *child(int index) { return _children[index]; }
    virtual int indexOfChild(const Client *child);
    virtual int activeChildIndex() { return _active_child_index; }
    virtual void setActiveChild(int index);
    virtual int maxTextHeight();
    virtual void redraw();

    // X11ServerWidget::EventHandler implementaion
    virtual void handleExpose() {
        redraw();
    }
    virtual void handleButtonPress(const XButtonEvent &ev);

    void reparent(X11ContainerContainer *p);
    void clear();
    // returns the index of the added client
    int addClient(X11Client *client);
    void removeClient(X11Client *client);

    X11ServerWidget *widget() { return _widget; }

    void handleClientMap(X11Client *client);

private:
    X11ServerWidget *currentWidget() {
        return isMinimized() ? _minimized_widget : _widget;
    }
    void applyMapState();

    std::vector<X11Client*> _children;
    int _active_child_index;
    X11ServerWidget *_widget, *_minimized_widget;
    bool _is_mapped;
};

#endif // __X11_CLIENT_CONTAINER_H__
