#include "client.h"

#include "container.h"

#include <stdio.h>
#include <iostream>

std::map<Window, Client*> Client::_client_from_window;

void Client::startup()
{
}

void Client::shutdown()
{
}

void Client::createClient(Window window, Display *dpy)
{
    printf("Client::windowCreated(): %d\n", (int)window);

    std::map<Window, Client*>::iterator it = _client_from_window.find(window);
    if (it != _client_from_window.end()) {
        printf("WARNING: client already created for window %d\n", (int)window);
    } else {
        XWindowAttributes attr;
        if (XGetWindowAttributes(dpy, window, &attr)) {
            if (attr.override_redirect) // dont't manage popups etc
                return;

            bool is_mapped = (attr.map_state == IsViewable);

            Client *c = new Client(window, dpy, is_mapped);
            _client_from_window[window] = c;

            Container::root()->addClient(c);
        } //FIXME else error
    }

    printf("_client_from_window.size(): %d\n", (int)_client_from_window.size());
}

void Client::windowDestroyed(Window window)
{
    printf("Client::windowDestroyed(): %d\n", (int)window);

    std::map<Window, Client*>::iterator it = _client_from_window.find(window);
    if (it != _client_from_window.end()) {
        Client *c = it->second;

        c->container()->removeClient(c);

        _client_from_window.erase(it);

        delete c;
    } else
        printf("WARNING: no client for destroyed window %d\n", (int)window);

    printf("_client_from_window.size(): %d\n", (int)_client_from_window.size());
}

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


Client::Client(Window w, Display *dpy, bool is_mapped) :
    _window(w), _container(0), _dpy(dpy), _is_mapped(is_mapped)
{
}

void Client::setContainer(Container *c)
{
    _container = c;
}

void Client::setRect(int x, int y, int w, int h)
{
    std::cout<<"FIXME - Client::setRect()\n";
    std::cout<<x<<","<<y<<","<<w<<","<<h<<"\n";

    XMoveResizeWindow(_dpy, _window, x, y, w, h);
}
