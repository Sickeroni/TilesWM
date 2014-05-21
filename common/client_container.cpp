#include "client_container.h"

#if 1

#include "client.h"
#include "container_container.h"
#include "client_container_layout.h"
#include "canvas.h"
#include "workspace.h"
#include "icon.h"
#include "colors.h"
#include "theme.h"
#include "common.h"

#include <sstream>
#include <string>
#include <stdlib.h>
#include <assert.h>


ClientContainer::ClientContainer(ContainerContainer *parent) : Container(CLIENT, parent),
//     _extra_space(0),
//     _custom_size_active(false),
    _layout(new ClientContainerLayout(this))
{
}

ClientContainer::~ClientContainer()
{
    delete _layout;
    _layout = 0;
}

ContainerLayout *ClientContainer::getLayout()
{
    return _layout;
}

#if 0
void ClientContainer::clear()
{
    for (Client *c = _clients.first(); c; ) {
        Client *remove_this = c;
        c = c->next();
        removeClient(remove_this);
        remove_this->setContainer(0);
    }
}
#endif

#if 0
Client *ClientContainer::activeClient()
{
    if (_active_client) {
//         std::cout<<"active client: \""<<_active_client->name()<<"\"\n";
        assert(_active_client->isMapped());
        return _active_client;
    } else
        return 0;
}
#endif

#if 0
void ClientContainer::setActiveClient(Client *client)
{
//     if (client)
//         std::cout<<"ClientContainer::setActiveClient(): \""<<client->name()<<"\"\n";
//     else
//         std::cout<<"ClientContainer::setActiveClient(): 0\n";

    assert(!client || client->isMapped());

    _active_client = client;

    if (_active_client)
        _active_client->raise();

    if (_active_client && !_active_client->hasFocus() && isActive())
        _active_client->setFocus();

    parent()->handleSizeHintsChanged(this);
}
#endif

void ClientContainer::handleMouseClick(int global_x, int global_y)
{
    //FIXME move this to mouse handler class

    printvar(global_x);
    printvar(global_y);

    int local_x = global_x;
    int local_y = global_y;
    globalToLocal(local_x, local_y);

    printvar(local_x);
    printvar(local_y);

    int clicked_tab_index = Theme::getTabAt(local_x, local_y, this);

    if (-1 < clicked_tab_index) {
        setActiveChild(clicked_tab_index);
        child(clicked_tab_index)->setFocus();
    }
}


// void ClientContainer::setExtraSpace(int space)
// {
//     _extra_space = space;
//     if (_extra_space < 0)
//         _extra_space = 0;
//     parent()->layout();
// }

// void ClientContainer::setCustomSizeActive(bool active)
// {
//     _custom_size_active = active;
// }

#if 0
void ClientContainer::handleClientMap(Client *client)
{
    if (!_active_client)
        setActiveClient(client);
    getLayout()->layoutContents();
}

void ClientContainer::handleClientUnmap(Client *client)
{
    if (client == _active_client)
        unfocusActiveClient();
    getLayout()->layoutContents();
}

void ClientContainer::handleClientAboutToBeMapped(Client *client)
{
}

void ClientContainer::handleClientFocusChange(Client *client)
{
    if (client->hasFocus())
        setActiveClient(client);
    redraw();
}
#endif

#if 0
void ClientContainer::handleClientSizeHintChanged(Client *client)
{
    if (activeClient() == client)
        parent()->handleSizeHintsChanged(this);
    getLayout()->layoutContents();
}
#endif

#if 0
void ClientContainer::focusPrevClient()
{
    if (_active_client) {
        for(Client *c = _active_client->prev(); c; c = c->prev()) {
            if (c->isMapped()) {
                setActiveClient(c);
                break;
            }
        }
    }
    redraw();
}

void ClientContainer::focusNextClient()
{
    if (_active_client) {
        for(Client *c = _active_client->next(); c; c = c->next()) {
            if (c->isMapped()) {
                setActiveClient(c);
                break;
            }
        }
    }
    redraw();
}

void ClientContainer::unfocusActiveClient()
{
    assert(_active_client);

    Client *old_active = _active_client;
    setActiveClient(0);

    for(Client *c = old_active->prev(); c; c = c->prev()) {
        if (c->isMapped()) {
            setActiveClient(c);
            break;
        }
    }

    if (!_active_client) {
        for(Client *c = old_active->next(); c; c = c->next()) {
            if (c->isMapped()) {
                setActiveClient(c);
                break;
            }
        }
    }

    redraw();
}
#endif

#if 0
void ClientContainer::addClient(Client *c)
{
    debug;

    if (ClientContainer *old_container = c->container()) {
        //FIXME  - TODO: unmap before and remap client after move
        debug<<"removing from old container ...";
        old_container->removeClientInt(c, true);
    }

    c->setContainer(this);

    _clients.append(c);

    printvar(_clients.count());

    if (c->isMapped() && !_active_client)
        setActiveClient(c);

    if (c->isMapped())
        getLayout()->layoutContents();
    else
        redraw();
}
#endif

#if 0
void ClientContainer::removeClientInt(Client *c, bool moving_to_new_container)
{
    debug;

    if (c == _active_client)
        unfocusActiveClient();

    _clients.remove(c);

    printvar(_clients.count());

    if (!moving_to_new_container)
        c->setContainer(0);

    if (c->isMapped())
        getLayout()->layoutContents();
    else
        redraw();

//FIXME
#if 0
    if (isEmpty() && _parent)
        _parent->setDirty(true);
#endif
}
#endif

#if 0
void ClientContainer::getClientRect(Rect &rect)
{
    int tabbar_height = calcTabbarHeight();

    static const int gap = 2;

    rect.x = _frame_width;
    rect.y = _frame_width + tabbar_height + gap;
    rect.w = width() - (2 * _frame_width);
    rect.h = height() - ((2 * _frame_width) + tabbar_height + gap);
}
#endif

#if 0
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

void ClientContainer::redrawAll()
{
    redraw();
}

#endif
