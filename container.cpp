#include "container.h"

#include "client.h"

#include <iostream>

Container *Container::_root = 0;

void Container::startup(int screen_width, int screen_height)
{
    _root = new Container(0, 0, 0, screen_width, screen_height);
}

void Container::shutdown()
{
    delete _root;
    _root = 0;
}

Container::Container(Container *parent, int x, int y, int w, int h) :
    _parent(parent), _orientation(HORIZONTAL),
    _x(x),  _y(y), _w(w), _h(h)
{
}

void Container::addClient(Client *c)
{
    if (c->container())
        c->container()->removeClient(c);

    _clients.push_back(c);

    c->setContainer(this);

    layout();
}

void Container::removeClient(Client *c)
{
    c->setContainer(0);

    for(std::list<Client*>::iterator it = _clients.begin(); it != _clients.end(); it++) {
        if (*it == c) {
            _clients.erase(it);
            break;
        }
    }
}

void Container::layout()
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

void Container::local_to_global(int &x, int &y)
{
    x += this->x();
    y += this->y();

    if (_parent)
        _parent->local_to_global(x, y);
}

void Container::moveClientRight()
{
    Client *current = 0;

    
}

Container *Container::east()
{
    if (_orientation == HORIZONTAL)
        return next();
    else {
        return _parent->east();
    }
}
