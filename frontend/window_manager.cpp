#include "window_manager.h"

#include "client_wrapper.h"

WindowManager::~WindowManager()
{
    assert(0);
}

void WindowManager::manageClient(Client *client)
{
    ClientWrapper *wrapper = new ClientWrapper(client, this);
    _clients.push_back(wrapper);
    manageClient(wrapper);
}

void WindowManager::unmanageClient(Client *client)
{
    assert(0);
}

void WindowManager::makeClientActive(Client *client)
{
    assert(0);
}

void WindowManager::setActive(bool active)
{
    _is_active = active;
    for (ClientWrapper *client : _clients)
        client->handleWindowManagerIsActiveChanged();
}
