#include "client_container.h"

#if 1

#include "client.h"
#include "container_container.h"

#include <iostream>
#include <stdlib.h>

int ClientContainer::_titlebar_height = 50;
int ClientContainer::_frame_width = 10;


ClientContainer::ClientContainer() : Container(CLIENT)
{
}

ClientContainer::~ClientContainer()
{
    for (Client *c = _clients.first(); c; ) {
        Client *remove_this = c;
        c = c->next();
        removeClient(remove_this);
    }
}

void ClientContainer::addClient(Client *c)
{
    if (c->container())
        c->container()->removeClient(c);

    _clients.append(c);

    c->setContainer(this);

    if (c->isMapped())
        layout();
}

void ClientContainer::removeClient(Client *c)
{
    c->setContainer(0);

    //TODO
    // if (c == _active_client)

    _clients.remove(c);

    if (isEmpty() && _parent)
        _parent->setDirty(true);
}

void ClientContainer::layout()
{
    std::cout<<"===================\nContainer::layout()";

//     if (!width() || !height())
//         return;

    int client_w = width() - (2 * _frame_width);
    int client_h = height() - ((2 * _frame_width) + _titlebar_height);

    if (!client_w || !client_h)
        return;

    int mapped_clients = numMappedClients();

    std::cout<<"mapped_clients: "<<mapped_clients<<"\n";

    if (!mapped_clients)
        return;

    int cell_width = 0, cell_height = 0;

    if (isHorizontal()) {
        cell_width = client_w / mapped_clients;
        cell_height = client_h;
    } else {
        cell_width = client_w;
        cell_height = client_h / mapped_clients;
    }

    std::cout<<"cell_width: "<<cell_width<<"\n";
    std::cout<<"cell_height: "<<cell_height<<"\n";
    std::cout<<"=================================\n";

    int i = 0;
    for(Client *c = _clients.first(); c; c = c->next()) {
        if (!c->isMapped())
            continue;

        int x = 0, y = 0;
        if (isHorizontal()) {
            x = i * cell_width + _frame_width;
            y = _frame_width + _titlebar_height;
        } else {
            x = _frame_width;
            y = i * cell_width + _frame_width + _titlebar_height;
        }

        localToGlobal(x, y);

        c->setRect(x, y, cell_width, cell_height);

        i++;
   }
}

ClientContainer *ClientContainer::splitContainer(Container *container, bool prepend_new_silbling)
{
    // create new parent
    ContainerContainer *new_parent = new ContainerContainer();

    if (container->parent()) {
        // replace this with new parent
        container->parent()->replaceChild(container, new_parent); // this de-parents/unlinks container
    } else
        _root = new_parent;

    ClientContainer *new_silbling = new ClientContainer();

    // add this + new child container to new parent
    if (prepend_new_silbling) {
        new_parent->appendChild(new_silbling);
        new_parent->appendChild(container);
    } else {
        new_parent->appendChild(container);
        new_parent->appendChild(new_silbling);
    }

    return new_silbling;
}


ClientContainer *ClientContainer::createSilblingFor(Container *container, bool prepend_new_silbling)
{
    ClientContainer *new_silbling = 0;

    if (container->parent()) {
        new_silbling = new ClientContainer();
        if (prepend_new_silbling)
            container->parent()->prependChild(new_silbling);
        else
            container->parent()->appendChild(new_silbling);
    } else
        new_silbling = splitContainer(container, prepend_new_silbling);

    return new_silbling;
}

ClientContainer *ClientContainer::getOrCreateSilblingFor(Container *container, bool get_prev)
{
    if (!get_prev && container->next())
        return container->next()->activeClientContainer();
    else if (get_prev && container->prev())
        return container->prev()->activeClientContainer();
    else
        return createSilblingFor(container, get_prev);
}

void ClientContainer::moveClientToOther(Client *client, Direction dir)
{
    if (client->container() != this)
        abort();

    if (!client->isMapped()) //FIXME is this ok ?
        return;

    bool backward = !isForwardDirection(dir);

    ClientContainer *target = 0;

    if (_parent) {
        if (orientationOfDirection(dir) == _parent->orientation()) // easy case
            target = getOrCreateSilblingFor(this, backward);

        else { // difficult case:
                 // if client container becomes empty -> use use silbling of parent container;
                 // else: 1. replace this with new parent container; 2. add this and new client container to parent created in step 1

            if (numMappedClients() <= 1) // cant't split - use use silbling of parent container
                target = getOrCreateSilblingFor(_parent, backward);
            else // split this
                target = splitContainer(this, backward);
        }
    } else {
        if (orientationOfDirection(dir) == _root_orientation)
            target = splitContainer(this, backward);
        else {
            target = new ClientContainer();

            ContainerContainer *subcontainer_this = new ContainerContainer();
            subcontainer_this->appendChild(this);

            ContainerContainer *subcontainer_silbling = new ContainerContainer();
            subcontainer_silbling->appendChild(target);

            ContainerContainer *new_root = new ContainerContainer();

            if (backward) {
                new_root->appendChild(subcontainer_silbling);
                new_root->appendChild(subcontainer_this);
            } else {
                new_root->appendChild(subcontainer_this);
                new_root->appendChild(subcontainer_silbling);
            }

            _root = new_root;
        }
    }

    target->addClient(client);
}

int ClientContainer::numMappedClients()
{
    int mapped_clients = 0;
    for(Client *c = _clients.first(); c; c = c->next()) {
        if (c->isMapped())
            mapped_clients++;
    }
    return mapped_clients;
}

#endif
