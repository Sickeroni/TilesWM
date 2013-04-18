#include "client.h"



#include "client_container.h"

#include <stdio.h>
#include <iostream>

#if 0



void Client::mapNotify(Window window)
{
    std::map<Window, Client*>::iterator it = _client_from_window.find(window);
    if (it != _client_from_window.end()) {
        Client *c = it->second;
        c->_is_mapped = true;
        c->container()->layout();
    }
}

void Client::unmapNotify(Window window)
{
    std::map<Window, Client*>::iterator it = _client_from_window.find(window);
    if (it != _client_from_window.end()) {
        Client *c = it->second;
        c->_is_mapped = false;
        c->container()->layout();
    }
}

#endif

/////////////////////////////////////////////////


Client::Client() :
    _container(0)
{
}

Client::~Client()
{
    std::cout<<"Client::~Client()\n";
    if (_container)
        _container->removeClient(this);
}
void Client::setContainer(ClientContainer *c)
{
    _container = c;
}

void Client::onMapStateChanged()
{
    if (_container)
        _container->layout();
}
