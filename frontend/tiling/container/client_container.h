#ifndef __CLIENT_CONTAINER_H__
#define __CLIENT_CONTAINER_H__

#include "container.h"
#include "client.h"
#include "common.h"

#include <vector>

class Canvas;
class ClientContainerLayout;
class ClientWrapper;

class ClientContainer final : public Container, public Client::EventHandler, public Client::PropertyListener
{
public:
    ClientContainer();
    virtual ~ClientContainer();

    virtual void draw(Canvas *canvas) override;

    virtual int numElements() const override { return _children.size(); }
    virtual ClientContainer *activeClientContainer() override { return this; }
    virtual bool isEmpty() const override { return _children.empty(); }
    virtual void redrawAll() override {
        redraw();
    }
    virtual ContainerLayout *getLayout() override;
    virtual bool isMinimized() const override { return _is_minimized; }

    // Client::EventHandler
    virtual void handleFocusChanged() {
        debug;
        redraw();
    }

    // Client::PropertyListener
    virtual void propertyChanged(Client *client, ClientBackend::Property property) override;

    ClientWrapper *child(size_t index) { return _children[index]; }
    int indexOfChild(const ClientWrapper *child);
    int activeChildIndex() { return _active_child_index; }
    // doesn't set focus to the client !
    void setActiveChild(int index);
    // return: index of added client
    int addChild(ClientWrapper *client);
    void removeChild(ClientWrapper *client);
    // deparents all children and puts them into <clients>
    void removeChildren(std::vector<ClientWrapper*> &clients);
    void setMinimized(bool minimized);
//     void clear();

    ClientWrapper *activeClient() {
        return activeChildIndex() != INVALID_INDEX ? child(activeChildIndex()) : 0;
    }

//     void handleClientFocusChange(Client *client);
//     void handleClientSizeHintChanged(Client *client);

private:
    ClientContainerLayout *_layout = 0;
    bool _is_minimized = false;
    std::vector<ClientWrapper*> _children;
    int _active_child_index = INVALID_INDEX;
};

#endif // __CLIENT_CONTAINER_H__
