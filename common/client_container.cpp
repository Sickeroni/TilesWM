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


ClientContainer::ClientContainer() : Container(CLIENT),
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
//         child(clicked_tab_index)->setFocus(); //FIXME focus model

//         assert(0);
        makeActive();
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

void ClientContainer::handleClientUnmap(Client *client)
{
    if (client == _active_client)
        unfocusActiveClient();
    getLayout()->layoutContents();
}


#endif

void ClientContainer::handleClientFocusChange(Client *client)
{
    redraw();
}

void ClientContainer::handleClientSizeHintChanged(Client *client)
{
    if (activeClient() == client)
        parent()->handleSizeHintsChanged(this);
    getLayout()->layoutContents();
}

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
