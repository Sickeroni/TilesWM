#include "window_manager.h"

#include "client_wrapper.h"

WindowManager::~WindowManager()
{
    assert(_clients.empty());
}

void WindowManager::manageClient(Client *client)
{
    ClientWrapper *wrapper = new ClientWrapper(client, this);
    _clients.push_back(wrapper);
    manageClient(wrapper);
}

void WindowManager::unmanageClient(Client *client)
{
    for (std::list<ClientWrapper*>::iterator it = _clients.begin();
            it != _clients.end();
            it++)
    {
        ClientWrapper *wrapper = *it;
        if (wrapper->client() == client) {
            _clients.erase(it);
            unmanageClient(wrapper);
            delete wrapper;
            return;
        }
    }
}

void WindowManager::makeClientActive(Client *client)
{
    for (std::list<ClientWrapper*>::iterator it = _clients.begin();
            it != _clients.end();
            it++)
    {
        ClientWrapper *wrapper = *it;
        if (wrapper->client() == client) {
            makeClientActive(wrapper);
            break;
        }
    }
}

void WindowManager::setActive(bool active)
{
    _is_active = active;
    for (ClientWrapper *client : _clients)
        client->handleWindowManagerIsActiveChanged();
}
