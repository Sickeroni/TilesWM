#include "client_container.h"

#if 1

#include "client.h"
#include "container_container.h"

#include <iostream>
#include <stdlib.h>

ClientContainer::ClientContainer() : Container(CLIENT)
{
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
    //FIXME
    abort();
}

#if 0

void ClientContainer::removeClient(Client *c)
{
    c->setContainer(0);

    for(std::list<Client*>::iterator it = _clients.begin(); it != _clients.end(); it++) {
        if (*it == c) {
            _clients.erase(it);
            break;
        }
    }

    if (isEmpty() && _parent)
        _parent->setDirty(true);
}

void ClientContainer::layout()
{
    std::cout<<"===================\nContainer::layout()";

    int mapped_clients = 0;
    for(std::list<Client*>::iterator it = _clients.begin(); it != _clients.end(); it++) {
        Client *c = *it;
        if (c->isMapped())
            mapped_clients++;
    }

    std::cout<<"mapped_clients: "<<mapped_clients<<"\n";


    if (!width() || !height())
        return;



    if (!mapped_clients)
        return;

    int cell_width = 0, cell_height = 0;

    if (isHorizontal()) {
        cell_width = width() / mapped_clients;
        cell_height = height();
    } else {
        cell_width = width();
        cell_height = height() / mapped_clients;
    }

    std::cout<<"cell_width: "<<cell_width<<"\n";
    std::cout<<"cell_height: "<<cell_height<<"\n";
    std::cout<<"=================================\n";

    int i = 0;
    for(std::list<Client*>::iterator it = _clients.begin(); it != _clients.end(); it++) {
        Client *c = *it;
        if (!c->isMapped())
            continue;

        int x = 0, y = 0;
        if (isHorizontal()) {
            x = i * cell_width;
            y = 0;
        } else {
            x = 0;
            y = i * cell_width;
        }

        local_to_global(x, y);

        c->setRect(x, y, cell_width, cell_height);

        i++;
   }
}

#endif


////////////////////////////


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

    //FIXME move client
    abort();
}

int ClientContainer::numMappedClients()
{
    //FIXME
    abort();
}

#endif
