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
#endif

Container::Index ClientContainer::indexOfChild(const ClientWrapper *child)
{
    for(Index i = 0; i < numElements(); i++) {
        if (child == _children[i])
            return i;
    }
    assert(false);
    abort();
}

void ClientContainer::setActiveChild(Index index)
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
    else
        getLayout()->layoutContents();
}

Container::Index ClientContainer::addChild(ClientWrapper *client)
{
    assert(_children.size() < MAX_INDEX);
    assert(!client->parent());

    client->reparent(this);
    client->setEventHandler(this);
    client->setPropertyListener(this);

    _children.push_back(client);

    client->setMapped(true);

    printvar(numElements());

    if (!activeClient()) {
        Index index = indexOfChild(client);
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
    Index index = indexOfChild(client);
    assert(index != INVALID_INDEX);

    bool was_active = (index == _active_child_index);

    client->setPropertyListener(0);
    client->setEventHandler(0);
    client->reparent(0);

    _children.erase(_children.begin() + make_signed<ptrdiff_t>(index));

    if (_active_child_index >= numElements())
        _active_child_index = numElements() -1;

    if (activeClient())
        activeClient()->raise();

    getLayout()->layoutContents();

    if (was_active && parentContainer())
        parentContainer()->handleSizeHintsChanged(this);
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

void ClientContainer::propertyChanged(Client *client, ClientBackend::Property property)
{
    if (property == ClientBackend::PROP_SIZE_HINTS &&
        activeClient() && activeClient()->client() == client)
    {
        if (parentContainer())
            parentContainer()->handleSizeHintsChanged(this);
        else
            getLayout()->layoutContents();
    } else
        redraw();
}
