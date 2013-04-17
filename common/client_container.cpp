#include "client_container.h"

#if 1

#include "client.h"
#include "container_container.h"

#include <iostream>
#include <stdlib.h>

int ClientContainer::_titlebar_height = 50;
int ClientContainer::_frame_width = 10;


ClientContainer::ClientContainer(ContainerContainer *parent) : Container(CLIENT, parent)
{
}

ClientContainer::~ClientContainer()
{
    clear();
}

void ClientContainer::clear()
{
    for (Client *c = _clients.first(); c; ) {
        Client *remove_this = c;
        c = c->next();
        removeClient(remove_this);
        remove_this->setContainer(0);
    }
}

void ClientContainer::addClient(Client *c)
{
    if (c->container())
        c->container()->removeClient(c);

    c->setContainer(this);

    _clients.append(c);

    //FIXME UGLY - layout client in advance
    {
        int client_w = width() - (2 * _frame_width);
        int client_h = height() - ((2 * _frame_width) + _titlebar_height);

        if (!client_w || !client_h)
            return;

        int mapped_clients = numMappedClients() + 1;

//         std::cout<<"mapped_clients: "<<mapped_clients<<"\n";

//         if (!mapped_clients)
//             return;

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

        Rect rect;
        rect.setSize(cell_width, cell_height);

        if (isHorizontal()) {
            rect.x = (mapped_clients-1) * cell_width + _frame_width;
            rect.y = _frame_width + _titlebar_height;
        } else {
            rect.x = _frame_width;
            rect.y = ((mapped_clients-1) * cell_height) + _frame_width + _titlebar_height;
        }

        c->setRect(rect);

    }

    if (c->isMapped())
        layout();
}


void ClientContainer::removeClient(Client *c)
{
    //TODO
    // if (c == _active_client)

    _clients.remove(c);

    if (isEmpty() && _parent)
        _parent->setDirty(true);
}


void ClientContainer::layout()
{
    std::cout<<"===================\nClientContainer::layout()";
    std::cout<<"is horizontal: "<<isHorizontal()<<'\n';

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

    Rect rect;
    rect.setSize(cell_width, cell_height);

    int i = 0;
    for(Client *c = _clients.first(); c; c = c->next()) {
        if (!c->isMapped())
            continue;

        if (isHorizontal()) {
            rect.x = i * cell_width + _frame_width;
            rect.y = _frame_width + _titlebar_height;
        } else {
            rect.x = _frame_width;
            rect.y = (i * cell_height) + _frame_width + _titlebar_height;
        }

//         localToGlobal(x, y);
        std::cout<<"x: "<<rect.x<<" y: "<<rect.y<<'\n';

        c->setRect(rect);

        i++;
   }
}

#if 0

ClientContainer *ClientContainer::splitContainer(Container *container, bool prepend_new_silbling)
{
    if (!container->parent())
        abort();

    return container->parent()->splitChild(container);
#if 0
    // create new parent
    ContainerContainer *new_parent = new ContainerContainer(container->parent());

    if (container->parent()) {
        // replace this with new parent
        container->parent()->replaceChild(container, new_parent); // unlinks container
    } else
        _root = new_parent;

    ClientContainer *new_silbling = new ClientContainer(new_parent);

    // add this + new child container to new parent
    if (prepend_new_silbling) {
        new_parent->appendChild(new_silbling);
        new_parent->appendChild(container);
    } else {
        new_parent->appendChild(container);
        new_parent->appendChild(new_silbling);
    }

    return new_silbling;
#endif
}
#endif
#if 0
ClientContainer *ClientContainer::createSilblingFor(Container *container, bool prepend_new_silbling)
{
    ClientContainer *new_silbling = 0;

    if (container->parent()) {
        new_silbling = container->parent()->createClientContainer();
        if (prepend_new_silbling)
            container->parent()->prependChild(new_silbling);
        else
            container->parent()->appendChild(new_silbling);
    } else
        abort();
//         new_silbling = splitContainer(container, prepend_new_silbling);

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
#endif

#if 0
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
        abort();
#if 0
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
#endif
    }

    target->addClient(client);
}
#endif

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
