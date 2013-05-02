#include "client_container.h"

#if 1

#include "client.h"
#include "container_container.h"
#include "canvas.h"

#include <iostream>
#include <sstream>
#include <string>
#include <stdlib.h>
#include <assert.h>


int ClientContainer::_titlebar_height = 50;
int ClientContainer::_frame_width = 10;


ClientContainer::ClientContainer(ContainerContainer *parent) : Container(CLIENT, parent),
    _mode(TABBED),
    _active_client(0)
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

Client *ClientContainer::activeClient()
{
    if (_active_client) {
//         std::cout<<"active client: \""<<_active_client->name()<<"\"\n";
        assert(_active_client->isMapped());
        return _active_client;
    } else
        return 0;
}

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

    if (_active_client && !_active_client->hasFocus() && hasFocus())
        _active_client->setFocus();
}

void ClientContainer::handleClientMap(Client *client)
{
    if (!_active_client)
        setActiveClient(client);
    layout();
}

void ClientContainer::handleClientUnmap(Client *client)
{
    if (client == _active_client)
        unfocusActiveClient();
    layout();
}

void ClientContainer::handleClientAboutToBeMapped(Client *client)
{
    if (_mode == STACKED)
        layoutStacked(client);
    redraw();
}

void ClientContainer::handleClientFocusChange(Client *client)
{
    if (client->hasFocus())
        setActiveClient(client);
    redraw();
}

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

void ClientContainer::addClient(Client *c)
{
    std::cout<<"ClientContainer::addClient()\n";

    if (ClientContainer *old_container = c->container()) {
        //FIXME  - TODO: unmap before and remap client after move
        std::cout<<"removing from old container ...\n";
        old_container->removeClientInt(c, true);
    }

    c->setContainer(this);

    _clients.append(c);

    std::cout<<"num clients: "<<_clients.count()<<'\n';


    if (c->isMapped() && !_active_client)
        setActiveClient(c);

#if 0
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
#endif
    if (c->isMapped())
        layout();
    else
        redraw();
}


void ClientContainer::removeClientInt(Client *c, bool moving_to_new_container)
{
    std::cout<<"ClientContainer::removeClient()\n";

    if (c == _active_client)
        unfocusActiveClient();

    _clients.remove(c);

    std::cout<<"num clients: "<<_clients.count()<<'\n';

    if (!moving_to_new_container)
        c->setContainer(0);

    if (c->isMapped())
        layout();
    else
        redraw();

    if (isEmpty() && _parent)
        _parent->setDirty(true);
}

void ClientContainer::draw(Canvas *canvas)
{
    switch(_mode) {
    case TABBED:
        drawTabbed(canvas);
        break;
    case STACKED:
        drawStacked(canvas);
        break;
    }

}

void ClientContainer::drawStacked(Canvas *canvas)
{
    Rect bg_rect = _rect;
    bg_rect.setPos(0, 0);
    canvas->erase(bg_rect);

    const int tabbar_border = 5;

    int tabbar_x = _frame_width + tabbar_border;
    int tabbar_y = _frame_width + tabbar_border;
    int tabbar_w = (width() - (2 * tabbar_border)) - (2 * _frame_width);
    int tabbar_h = _titlebar_height - (2 * tabbar_border);

    Rect tabbar_rect;
    tabbar_rect.set(tabbar_x, tabbar_y, tabbar_w, tabbar_h - 5);


    std::stringstream title;

    if (activeClient())
        title << "Active client: " << activeClient()->name();
    else
        title << "No active client";


    int mapped_clients = numMappedClients();

    title<<"  |  Clients: "<<_clients.count()<<"  |  Mapped: "<<mapped_clients;

    canvas->drawText(title.str().c_str(), tabbar_rect, 0xFFFFFF, 0x0);

    int client_w = width() - (2 * _frame_width);
    int client_h = height() - ((2 * _frame_width) + _titlebar_height);

    if (!client_w || !client_h)
        return;

//     std::cout<<"mapped_clients: "<<mapped_clients<<"\n";

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

//     std::cout<<"cell_width: "<<cell_width<<"\n";
//     std::cout<<"cell_height: "<<cell_height<<"\n";
//     std::cout<<"=================================\n";

    Rect rect;
    rect.setSize(cell_width, cell_height);

    const int gap = 5;

    rect.w -= 2 * gap;
    rect.h -= 2 * gap;

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

        rect.x += gap;
        rect.y += gap;

        if (c->hasFocus()) {
            Rect focus_rect;
            focus_rect.set(rect.x+2, rect.y+2, rect.w-4, rect.h-4);
            canvas->drawFrame(focus_rect, 0xFF8080);
        }

//         localToGlobal(x, y);
//         std::cout<<"x: "<<rect.x<<" y: "<<rect.y<<'\n';

        if (activeClient() == c) {
            canvas->drawFrame(rect, 0x0);
        } else {
//             canvas->drawFrame(rect, 0x0);
        }

        i++;
   }
}

void ClientContainer::drawTabbed(Canvas *canvas)
{
    std::cout<<"ClientContainer::draw()\n";

    Rect bg_rect = _rect;
    bg_rect.setPos(0, 0);
    canvas->erase(bg_rect);


    const int tabbar_border = 5;

//     int tab_border = 2;

    int num_tabs = _clients.count(); //numMappedClients();

    int tabbar_x = _frame_width + tabbar_border;
    int tabbar_y = _frame_width + tabbar_border;
    int tabbar_w = (width() - (2 * tabbar_border)) - (2 * _frame_width);
    int tabbar_h = _titlebar_height - (2 * tabbar_border);

    Rect tabbar_rect;
    tabbar_rect.set(tabbar_x, tabbar_y, tabbar_w, tabbar_h);


    if (!num_tabs)
        return;

//     int baseline = (_frame_width + _titlebar_height) - tab_border:

    int tab_width = tabbar_w / num_tabs;
    int tab_height = tabbar_h;

    int i = 0;
    for(Client *c = _clients.first(); c; c = c->next()) {
//         std::string text;
        Rect r;
        r.set(tabbar_x + (i * tab_width), tabbar_y, tab_width, tab_height);

        r.x+=2;
        r.y+=2;
        r.w-=4;
        r.h-=4;

        unsigned long fg, bg;

        bg = 0x0;

        if (activeClient() == c)
            fg = 0x00FF00;
        else
            fg = 0xAAAAAA;

        canvas->drawFrame(r, (activeClient() == c) ? 0x222299 : 0x222222);

        #if 0
        if (activeClient() == c) {
            Rect tmp = r;
            tmp.x+=2;
            tmp.y+=2;
            tmp.w-=4;
            tmp.h-=4;
            canvas->drawFrame(tmp,  0x000088);
        }
        #endif

        r.x+=10;
        r.y-= 10;

        canvas->drawText(c->name().c_str(), r, fg, bg);

        i++;
    }

}


void ClientContainer::layout()
{
    switch(_mode) {
    case TABBED:
        layoutTabbed();
        break;
    case STACKED:
        layoutStacked(0);
        break;
    }
    redraw();
}

void ClientContainer::layoutTabbed()
{
#if 0
    int client_w = width() - (2 * _frame_width);
    int client_h = height() - ((2 * _frame_width) + _titlebar_height);

    int tab_width = client_w / mapped_clients;
    int tab_height = (_titlebar_height - 2) - _frame_width;
#endif

//     layoutStacked(0);


    const int tabbar_border = 5;

//     int tab_border = 2;

//     int num_tabs = _clients.count(); //numMappedClients();

    int tabbar_x = _frame_width + tabbar_border;
    int tabbar_y = _frame_width + tabbar_border;
    int tabbar_w = (width() - (2 * tabbar_border)) - (2 * _frame_width);
    int tabbar_h = _titlebar_height - (2 * tabbar_border);

    Rect tabbar_rect;
    tabbar_rect.set(tabbar_x, tabbar_y, tabbar_w, tabbar_h);

    int client_w = width() - (2 * _frame_width);
    int client_h = height() - ((2 * _frame_width) + _titlebar_height);

    Rect client_rect;
    client_rect.set(_frame_width, _frame_width + _titlebar_height, client_w, client_h);

    if (activeClient())
        activeClient()->raise();
    for(Client *c = _clients.first(); c; c = c->next())
        c->setRect(client_rect);
}

void ClientContainer::getClientSize(int &w, int &h)
{
    //FIXME - this works only for stacked layout

    const int cell_border = 12; //FIXME

    getStackCellSize(numMappedClients(), w, h);

    w -= 2 * cell_border;
    h -= 2 * cell_border;

}

void ClientContainer::getStackCellSize(int num_cells, int &w, int &h)
{
    int client_w = width() - (2 * _frame_width);
    int client_h = height() - ((2 * _frame_width) + _titlebar_height);

    if (!client_w || !client_h)
        return;


//     std::cout<<"mapped_clients: "<<mapped_clients<<"\n";

    if (!num_cells)
        num_cells = 1;

    int cell_width = 0, cell_height = 0;

    if (isHorizontal()) {
        cell_width = client_w / num_cells;
        cell_height = client_h;
    } else {
        cell_width = client_w;
        cell_height = client_h / num_cells;
    }

    w = cell_width;
    h = cell_height;

}

void ClientContainer::layoutStacked(Client *about_to_be_mapped)
{
    std::cout<<"======================\nClientContainer::layout()\n";
    std::cout<<"is horizontal: "<<isHorizontal()<<'\n';

    const int cell_border = 12; //FIXME

    int num_cells = numMappedClients();
    if (about_to_be_mapped)
        num_cells++;

    int cell_width, cell_height;
    getStackCellSize(num_cells, cell_width, cell_height);

    Rect rect;
    rect.setSize(cell_width, cell_height);

    rect.w -= 2 * cell_border;
    rect.h -= 2 * cell_border;

    int i = 0;
    for(Client *c = _clients.first(); c; c = c->next()) {
        if (!c->isMapped() && c != about_to_be_mapped)
            continue;

        if (isHorizontal()) {
            rect.x = (i * cell_width) + _frame_width;
            rect.y = _frame_width + _titlebar_height;
        } else {
            rect.x = _frame_width;
            rect.y = (i * cell_height) + _frame_width + _titlebar_height;
        }

        rect.x += cell_border;
        rect.y += cell_border;

//         std::cout<<"x: "<<rect.x<<" y: "<<rect.y<<'\n';

        c->setRect(rect);

        i++;
   }
}


void ClientContainer::focusSilbling(Direction where)
{
    ContainerContainer *parent = 0;
    if (orientationOfDirection(where) == _parent->orientation())
        parent = _parent;
    else
        parent = _parent->parent();

    if (parent) {
        if (isForwardDirection(where))
            parent->focusNextChild();
        else
            parent->focusPrevChild();
    }
}

void ClientContainer::createSilbling(Direction where)
{
    std::cout<<"ClientContainer::createSilbling()\n";
    bool prepend = !isForwardDirection(where);
    if (orientationOfDirection(where) == _parent->orientation()) {
        _parent->addNewClientContainer(prepend);
    } else
        _parent->splitChild(this, prepend);
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
#if 1
ClientContainer *ClientContainer::createSilblingFor(Container *container, bool prepend_new_silbling)
{
    ClientContainer *new_silbling = 0;

    if (container->parent())
        new_silbling = container->parent()->addNewClientContainer(prepend_new_silbling);

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

ClientContainer *ClientContainer::getSilbling(bool get_prev)
{
    if (get_prev && prev())
        return prev()->activeClientContainer();
    else if (!get_prev && next())
        return next()->activeClientContainer();
    else
        return 0;
}

#if 1
void ClientContainer::moveClientToOther(Client *client, Direction dir)
{
    if (client->container() != this)
        abort();

    if (!client->isMapped()) //FIXME is this ok ?
        return;

    bool backward = !isForwardDirection(dir);

    ClientContainer *target = 0;

    if (_parent) {
        if (orientationOfDirection(dir) == _parent->orientation()) { // easy case
            if (numMappedClients() > 1) // only create new direct silbling if container doesn't become empty
                target = getOrCreateSilblingFor(this, backward);
            else
                target = getSilbling(backward);
        }

        else { // difficult case:
                 // if client container becomes empty -> use use silbling of parent container;
                 // else: 1. replace this with new parent container; 2. add this and new client container to parent created in step 1

            if (numMappedClients() <= 1) // cant't split - use use silbling of parent container
                target = getOrCreateSilblingFor(_parent, backward);
            else { // split this
//                 target = splitContainer(this, backward);
                target = _parent->splitChild(this, backward);

                if (!target) { // split failed - maximum hierarchy depth exceeded ?
                    target = getOrCreateSilblingFor(_parent, backward);
                }
            }
        }
    } else {
        abort();
    }

    if (target) {
        target->addClient(client);
        target->setActiveClient(client);
        target->makeActive();
//         target->setFocus();
        ContainerContainer *root = 0;
        for (ContainerContainer *c = target->parent(); c; c = c->parent()) {
            root = c;
        }

        root->setFocus();
        root->redrawAll();
    }
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

void ClientContainer::redrawAll()
{
    redraw();
}

#endif
