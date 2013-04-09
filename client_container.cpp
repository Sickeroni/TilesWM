#include "client_container.h"

#include "client.h"

#include <iostream>


ClientContainer::ClientContainer(ContainerContainer *parent, int x, int y, int w, int h) :
    Container(parent, x, y, w, h)
{
}

void ClientContainer::addClient(Client *c)
{
    if (c->container())
        c->container()->removeClient(c);

    _clients.push_back(c);

    c->setContainer(this);

    if (c->isMapped())
        layout();
}

void ClientContainer::removeClient(Client *c)
{
    c->setContainer(0);

    for(std::list<Client*>::iterator it = _clients.begin(); it != _clients.end(); it++) {
        if (*it == c) {
            _clients.erase(it);
            break;
        }
    }
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

    if (_orientation == HORIZONTAL) {
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
        if (_orientation == HORIZONTAL) {
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

ClientContainer *ClientContainer::getOrCreateSilbling(Direction dir)
{
    if (ClientContainer *s = findSilbling(dir))
        return s;
    else {
        if (_parent) {
            return _parent->getOrCreateSilblingOf(this, dir);
        } else {
            //ASSERT(this == root);

            bool push_front = dir == WEST || dir == NORTH;


            if (defaultOrientation() == orientationOfDirection(dir)) {
                // direct

                ContainerContainer *new_root =
                    new ContainerContainer(0 ,0, 0, _root->width(), _root->height());
                ClientContainer *new_silbling =
                    new ClientContainer(new_root, 0, 0, _root->width(), _root->height());

                if (push_front) {
                    new_root->addContainer(new_silbling);
                    new_root->addContainer(this);
                } else {
                    new_root->addContainer(this);
                    new_root->addContainer(new_silbling);
                }

                _root = new_root;
                return new_silbling;

            } else {
                // indirect

                ContainerContainer *new_root =
                    new ContainerContainer(0 ,0, 0, _root->width(), _root->height());

                ClientContainer *new_silbling =
                    new ClientContainer(new_root, 0, 0, _root->width(), _root->height());

                ContainerContainer *subcontainer_this = new ContainerContainer(0 ,0, 0, _root->width(), _root->height());
                subcontainer_this->addContainer(this);

                ContainerContainer *subcontainer_silbling = new ContainerContainer(0 ,0, 0, _root->width(), _root->height());
                subcontainer_silbling->addContainer(new_silbling);

                if (push_front) {
                    new_root->addContainer(subcontainer_silbling);
                    new_root->addContainer(subcontainer_this);
                } else {
                    new_root->addContainer(subcontainer_this);
                    new_root->addContainer(subcontainer_silbling);
                }

                _root = new_root;
                return new_silbling;
            }
        }
    }
}
