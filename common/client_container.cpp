#include "client_container.h"

#include "client.h"
#include "container_container.h"
#include "container_widget.h"
#include "client_container_layout.h"
#include "canvas.h"
#include "workspace.h"
#include "icon.h"
#include "colors.h"
#include "theme.h"
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

void ClientContainer::clear()
{
    _active_child_index = INVALID_INDEX;

    for(Client *child : _children)
        child->setContainer(0);
    _children.clear();
}

ContainerLayout *ClientContainer::getLayout()
{
    return _layout;
}

void ClientContainer::handleClientFocusChange(Client *client)
{
    redraw();
}

void ClientContainer::handleClientSizeHintChanged(Client *client)
{
    if (activeClient() == client)
        parent()->handleSizeHintsChanged(this);
    getLayout()->layoutContents();
}

void ClientContainer::setMinimized(bool minimized)
{
    _is_minimized = minimized;
    widget()->setMinimized(minimized);
}

int ClientContainer::indexOfChild(const Client *child)
{
    for(int i = 0; i < numElements(); i++) {
        if (child == _children[i])
            return i;
    }
    assert(false);
    abort();
}

void ClientContainer::handleClientMap(Client *client)
{
    if (!activeClient()) {
        int index = indexOfChild(client);
        assert(index != INVALID_INDEX);
        setActiveChild(index);
    }
    getLayout()->layoutContents();
}

void ClientContainer::setActiveChild(int index)
{
    printvar(index);
//     if (client)
//         std::cout<<"ClientContainer::setActiveClient(): \""<<client->name()<<"\"\n";
//     else
//         std::cout<<"ClientContainer::setActiveClient(): 0\n";

    assert(index < numElements());
    assert(index == INVALID_INDEX || _children[index]->isMapped());

    _active_child_index = index;

    if (activeClient())
        activeClient()->raise();

    if (parent())
        parent()->handleSizeHintsChanged(this);
}

int ClientContainer::addChild(Client *client)
{
    assert(!client->container());

    client->setContainer(this);

    // make sure the active client stays on top of the stacking order
    if (activeClient())
        activeClient()->raise();

    _children.push_back(client);

    printvar(numElements());

    getLayout()->layoutContents();

    return numElements() - 1;
}

void ClientContainer::removeChild(Client *client)
{
    int index = indexOfChild(client);
    assert(index >= 0);

    client->setContainer(0);

    _children.erase(_children.begin() + index);

    if (_active_child_index >= numElements())
        _active_child_index = numElements() -1;

    if (activeClient())
        activeClient()->raise();

    getLayout()->layoutContents();
}

void ClientContainer::removeChildren(std::vector<Client*> &clients)
{
    for(Client *child : _children)
        clients.push_back(child);
    clear();
}
