#ifndef __X11_CLIENT_CONTAINER_H__
#define __X11_CLIENT_CONTAINER_H__

#include "client_container.h"

#include "x11_server_widget.h"
#include "x11_client.h"

#include <vector>

class X11ContainerContainer;
class X11Client;

class X11ClientContainer final : public ClientContainer, public X11ServerWidget::EventHandler
{
public:
    X11ClientContainer();
    ~X11ClientContainer();

    virtual void setRect(const Rect &rect) override;
    virtual void handleMaximizedChanged() override;
    virtual void handleActiveChanged() override;
    virtual int numElements() override { return _children.size(); }
    virtual bool isEmpty() override { return _children.empty(); }
    virtual Client *child(size_t index) { return _children[index]; }
    virtual int indexOfChild(const Client *child) override;
    virtual int activeChildIndex() override { return _active_child_index; }
    virtual void setActiveChild(int index) override;
    virtual int addChild(Client *client) override;
    virtual void removeChild(Client *client) override;
    virtual void removeChildren(std::vector<Client*> &clients) override;
    virtual int maxTextHeight() override;
    virtual void redraw() override;
    virtual void setMapped(bool mapped) override;

    // X11ServerWidget::EventHandler implementaion
    virtual void handleExpose() override {
        redraw();
    }
    virtual void handleButtonPress(const XButtonEvent &ev) override;

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
