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
    struct ClientDragHandler {
        virtual void handleClientDragStart(ClientWrapper *client,  int x_global, int y_global, Client::DragMode mode) = 0;
    };

    ClientContainer();
    virtual ~ClientContainer();

    virtual void draw(Canvas *canvas) override;

    virtual Size numElements() const override { return _children.size(); }
    virtual ClientContainer *activeClientContainer() override { return this; }
    virtual bool isEmpty() const override { return _children.empty(); }
    virtual void redrawAll() override {
        redraw();
    }
    virtual ContainerLayout *getLayout() override;

    // Client::EventHandler
    virtual void handleDragStart(int x_global, int y_global, Client::DragMode mode) override;
    virtual void handleFocusChanged() {
        debug;
        redraw();
    }

    // Client::PropertyListener
    virtual void propertyChanged(Client *client, ClientBackend::Property property) override;

    ClientWrapper *child(Index index) { return _children[index]; }
    Index indexOfChild(const ClientWrapper *child);
    Index activeChildIndex() { return _active_child_index; }
    // doesn't set focus to the client !
    void setActiveChild(Index index);
    // return: index of added client
    Index addChild(ClientWrapper *client);
    void removeChild(ClientWrapper *client);
    // deparents all children and puts them into <clients>
    void removeChildren(std::vector<ClientWrapper*> &clients);
//     void clear();
    void setClientDragHandler(ClientDragHandler *handler) {
        _client_drag_handler = handler;
    }

    ClientWrapper *activeClient() {
        return activeChildIndex() != INVALID_INDEX ? child(activeChildIndex()) : 0;
    }

//     void handleClientFocusChange(Client *client);
//     void handleClientSizeHintChanged(Client *client);

private:
    ClientContainerLayout *_layout = 0;
    std::vector<ClientWrapper*> _children;
    Index _active_child_index = INVALID_INDEX;
    ClientDragHandler *_client_drag_handler = 0;
};

#endif // __CLIENT_CONTAINER_H__
