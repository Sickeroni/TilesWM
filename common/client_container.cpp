#include "client_container.h"

#if 1

#include "client.h"
#include "container_container.h"
#include "canvas.h"
#include "workspace.h"
#include "icon.h"

#include <iostream>
#include <sstream>
#include <string>
#include <stdlib.h>
#include <assert.h>



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

int ClientContainer::minimumWidth()
{
    return _vertical_tabbar_width + (2 * _frame_width);
}

int ClientContainer::minimumHeight()
{
    int tabbar_height = 0;
    if (isMinimized()) {
        if (isHorizontal())
            tabbar_height = calcTabbarHeight();
        else // vertical tabbar
            tabbar_height = _clients.count() * calcTabbarHeight();
    } else
        tabbar_height = calcTabbarHeight();

    return tabbar_height + (2 * _frame_width);;
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
    if (isMinimized()) {
        if (isHorizontal())
            drawTabs(canvas);
        else
            drawVerticalTabs(canvas);
    } else {
        switch(_mode) {
        case TABBED:
            drawTabs(canvas);
            break;
        case STACKED:
            drawStacked(canvas);
            break;
        }
    }
}

void ClientContainer::drawStacked(Canvas *canvas)
{
    Rect bg_rect = _rect;
    bg_rect.setPos(0, 0);
    canvas->erase(bg_rect);

    Rect tabbar_rect;
    getTabbbarRect(tabbar_rect);
    int tabbar_height = tabbar_rect.h;


    std::stringstream title;

    if (activeClient())
        title << "Active client: " << activeClient()->name();
    else
        title << "No active client";


    int mapped_clients = numMappedClients();

    title<<"  |  Clients: "<<_clients.count()<<"  |  Mapped: "<<mapped_clients;

    canvas->drawText(title.str().c_str(), tabbar_rect, 0xFFFFFF, 0x0);

    Rect client_rect;
    getClientRect(client_rect);

    if (!client_rect.w || !client_rect.h)
        return;

//     std::cout<<"mapped_clients: "<<mapped_clients<<"\n";

    if (!mapped_clients)
        return;

    int cell_width = 0, cell_height = 0;

    if (isHorizontal()) {
        cell_width = client_rect.w / mapped_clients;
        cell_height = client_rect.h;
    } else {
        cell_width = client_rect.w;
        cell_height = client_rect.h / mapped_clients;
    }

//     std::cout<<"cell_width: "<<cell_width<<"\n";
//     std::cout<<"cell_height: "<<cell_height<<"\n";
//     std::cout<<"=================================\n";

    Rect rect;
    rect.setSize(cell_width, cell_height);

    static const int gap = 5;

    rect.w -= 2 * gap;
    rect.h -= 2 * gap;

    int i = 0;
    for(Client *c = _clients.first(); c; c = c->next()) {
        if (!c->isMapped())
            continue;

        if (isHorizontal()) {
            rect.x = i * cell_width + _frame_width;
            rect.y = _frame_width + tabbar_height;
        } else {
            rect.x = _frame_width;
            rect.y = (i * cell_height) + _frame_width + tabbar_height;
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

void ClientContainer::drawVerticalTabs(Canvas *canvas)
{
    Rect bg_rect = _rect;
    bg_rect.setPos(0, 0);
    canvas->erase(bg_rect);

    int tabbar_height = calcTabbarHeight();

    int i = 0;
    for (Client *c = _clients.first(); c; c = c->next()) {
        Rect tab_rect(0, i * tabbar_height, _vertical_tabbar_width, tabbar_height);

        canvas->drawFrame(tab_rect, (activeClient() == c) ? 0x222299 : 0x222222);

        if (c->icon()) {
            int icon_x = tab_rect.x + 2;
            int icon_y = tab_rect.y + 2;
            canvas->drawIcon(c->icon(), icon_x, icon_y);
        }

        Rect text_rect = tab_rect;

        text_rect.x+=30;
        text_rect.y-= 10;

        uint32 fg = 0, bg = 0;

        if (activeClient() == c)
            fg = 0x00FF00;
        else
            fg = 0xAAAAAA;

        canvas->drawText(c->name().c_str(), text_rect, fg, bg);


        i++;
    }
}

void ClientContainer::drawTabs(Canvas *canvas)
{
    std::cout<<"ClientContainer::draw()\n";

    const int gap = 2;

    Rect bg_rect = _rect;
    bg_rect.setPos(0, 0);
    canvas->erase(bg_rect);

    int num_tabs = _clients.count(); //numMappedClients();
    if (!num_tabs)
        return;

    Rect tabbar_rect;
    getTabbbarRect(tabbar_rect);

    int num_gaps = num_tabs - 1;

    int tab_width = (tabbar_rect.w - (num_gaps * gap)) / num_tabs;
    int tab_height = tabbar_rect.h;

    int max_text_height = maxTextHeight();

    int i = 0;
    for(Client *c = _clients.first(); c; c = c->next()) {
        Rect tab_rect(tabbar_rect.x + (i * (tab_width + gap)), tabbar_rect.y, tab_width, tab_height);

        Rect frame_rect = tab_rect;

        canvas->drawFrame(frame_rect, (activeClient() == c) ? 0x222299 : 0x444444);

        if (c->icon()) {
            int icon_x = tab_rect.x + _tab_inner_margin;
            int icon_y = tab_rect.y + _tab_inner_margin;
            canvas->drawIcon(c->icon(), icon_x, icon_y);
        }

        Rect text_rect(tab_rect.x + _tab_inner_margin, tab_rect.y + _tab_inner_margin, 100, max_text_height);

        if (c->icon())
            text_rect.x += (c->icon()->width() + 5);

        canvas->drawText(c->name(), text_rect, activeClient() == c ? 0x0 : 0x666666, 0x0);

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

int ClientContainer::calcTabbarHeight()
{
    return maxTextHeight() + (2 * _tab_inner_margin);
}

void ClientContainer::getTabbbarRect(Rect &rect)
{
    int tabbar_x = _frame_width;
    int tabbar_y = _frame_width;
    int tabbar_w = width() - (2 * _frame_width);
    int tabbar_h = calcTabbarHeight();

    rect.set(tabbar_x, tabbar_y, tabbar_w, tabbar_h);
}

void ClientContainer::getClientRect(Rect &rect)
{
    int tabbar_height = calcTabbarHeight();

    static const int gap = 2;

    rect.x = _frame_width;
    rect.y = _frame_width + tabbar_height + gap;
    rect.w = width() - (2 * _frame_width);
    rect.h = height() - ((2 * _frame_width) + tabbar_height + gap);
}

void ClientContainer::layoutTabbed()
{
    Rect client_rect;
    getClientRect(client_rect);

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
    Rect client_rect;
    getClientRect(client_rect);

    if (!client_rect.w || !client_rect.h)
        return;

//     std::cout<<"mapped_clients: "<<mapped_clients<<"\n";

    if (!num_cells)
        num_cells = 1;

    int cell_width = 0, cell_height = 0;

    if (isHorizontal()) {
        cell_width = client_rect.w / num_cells;
        cell_height = client_rect.h;
    } else {
        cell_width = client_rect.w;
        cell_height = client_rect.h / num_cells;
    }

    w = cell_width;
    h = cell_height;

}

void ClientContainer::layoutStacked(Client *about_to_be_mapped)
{
    std::cout<<"======================\nClientContainer::layout()\n";
    std::cout<<"is horizontal: "<<isHorizontal()<<'\n';

    const int cell_border = 12; //FIXME
    int tabbar_height = calcTabbarHeight();

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
            rect.y = _frame_width + tabbar_height;
        } else {
            rect.x = _frame_width;
            rect.y = (i * cell_height) + _frame_width + tabbar_height;
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

ClientContainer *ClientContainer::getSilbling(bool get_prev)
{
    if (get_prev && prev())
        return prev()->activeClientContainer();
    else if (!get_prev && next())
        return next()->activeClientContainer();
    else
        return 0;
}

void ClientContainer::moveClientToOther(Client *client, Direction dir)
{
    if (workspace()->maximized())
        return;

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
        client->setFocus();
//         target->setFocus();
//         ContainerContainer *root = 0;
//         for (ContainerContainer *c = target->parent(); c; c = c->parent()) {
//             root = c;
//         }

//         root->setFocus();
//         root->redrawAll();
    }
}

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
