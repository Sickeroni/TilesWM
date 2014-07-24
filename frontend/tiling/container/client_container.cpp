#include "client_container.h"

#include "client_wrapper.h"
#include "client_container_layout.h"
#include "client_container_theme.h"
#include "widget_backend.h"
#include "application.h"
#include "common.h"

#include <sstream>
#include <string>
#include <stdlib.h>
#include <assert.h>


ClientContainer::ClientContainer() : Container(CLIENT),
    _layout(new ClientContainerLayout(this))
{
}

ClientContainer::~ClientContainer()
{
    assert(_children.empty());
    delete _layout;
    _layout = 0;
}

ContainerLayout *ClientContainer::getLayout()
{
    return _layout;
}

#if 0
void ClientContainer::handleClientFocusChange(ClientWrapper *client)
{
    redraw();
}

void ClientContainer::handleClientSizeHintChanged(ClientWrapper *client)
{
    if (activeClient() == client) {
        if (parentContainer())
            parentContainer()->handleSizeHintsChanged(this);
        else
            getLayout()->layoutContents();
    }
}
#endif

void ClientContainer::setMinimized(bool minimized)
{
    _is_minimized = minimized;
    _backend->setMinimized(minimized);
}

int ClientContainer::indexOfChild(const ClientWrapper *child)
{
    for(int i = 0; i < numElements(); i++) {
        if (child == _children[i])
            return i;
    }
    assert(false);
    abort();
}

void ClientContainer::setActiveChild(int index)
{
    printvar(index);
//     if (client)
//         std::cout<<"ClientContainer::setActiveClient(): \""<<client->name()<<"\"\n";
//     else
//         std::cout<<"ClientContainer::setActiveClient(): 0\n";

    assert(index < numElements());
//     assert(index == INVALID_INDEX || _children[index]->isMapped());

    _active_child_index = index;

    if (activeClient())
        activeClient()->raise();

    if (parentContainer())
        parentContainer()->handleSizeHintsChanged(this);
}

int ClientContainer::addChild(ClientWrapper *client)
{
    assert(!client->parent());

    client->reparent(this, _backend);
    client->setDragHandler(this);

    _children.push_back(client);

    client->setMapped(true);

    printvar(numElements());

    if (!activeClient()) {
        int index = indexOfChild(client);
        assert(index != INVALID_INDEX);
        setActiveChild(index);
    }

    // make sure the active client stays on top of the stacking order
    if (activeClient())
        activeClient()->raise();

    getLayout()->layoutContents();

    return numElements() - 1;
}

void ClientContainer::removeChild(ClientWrapper *client)
{
    int index = indexOfChild(client);
    assert(index >= 0);

    client->setDragHandler(0);
    client->reparent(0, 0);

    _children.erase(_children.begin() + index);

    if (_active_child_index >= numElements())
        _active_child_index = numElements() -1;

    if (activeClient())
        activeClient()->raise();

    getLayout()->layoutContents();
}

#if 0
void ClientContainer::removeChildren(std::vector<ClientWrapper*> &clients)
{
    _active_child_index = INVALID_INDEX;

    clients.reserve(_children.size());

    for(ClientWrapper *child : _children) {
        child->reparent(0, 0);
        clients.push_back(child);
    }
    _children.clear();
}
#endif

void ClientContainer::draw(Canvas *canvas)
{
    Theme::drawClientContainer(this, canvas);
}
