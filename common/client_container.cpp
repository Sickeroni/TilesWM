#include "client_container.h"

#if 1

#include "client.h"
#include "container_container.h"
#include "canvas.h"
#include "workspace.h"
#include "icon.h"
#include "colors.h"
#include "common.h"

#include <sstream>
#include <string>
#include <stdlib.h>
#include <assert.h>


ClientContainer::ClientContainer(ContainerContainer *parent) : Container(CLIENT, parent),
//     _extra_space(0),
//     _custom_size_active(false),
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

    parent()->handleSizeHintsChanged(this);
}

int ClientContainer::minWidthInt()
{
    int min_w = _vertical_tabbar_width;
    if (!isMinimized() && activeClient() && activeClient()->minWidth() > min_w)
        min_w = activeClient()->minWidth();
    return min_w + (2 * _frame_width);
}

int ClientContainer::minHeightInt()
{
    int tabbar_height = 0;
    if (isMinimized()) {
        if (isHorizontal())
            tabbar_height = calcTabbarHeight();
        else // vertical tabbar
            tabbar_height = (_clients.count() * calcTabbarHeight()
                             + ((!_clients.count() ? 0 : _clients.count() - 1) * _tab_gap));
    } else
        tabbar_height = calcTabbarHeight();
	
	int min_client_height = 0;
	if (!isMinimized() && activeClient())
		min_client_height = activeClient()->minHeight();

    return tabbar_height + min_client_height + (2 * _frame_width);
}

void ClientContainer::handleMouseClick(int global_x, int global_y)
{
    printvar(global_x);
    printvar(global_y);

    int local_x = global_x;
    int local_y = global_y;
    globalToLocal(local_x, local_y);

    printvar(local_x);
    printvar(local_y);

    Rect tabbar_rect;
    getTabbbarRect(tabbar_rect);

    printvar(tabbar_rect.y);
    printvar(tabbar_rect.h);

    if (tabbar_rect.isPointInside(local_x, local_y)) {
//         std::cout<<"inside tabbar.\n";
        int tab_width, tab_height;
        getTabSize(tab_width, tab_height);

        int i = 0;
        for(Client *c = _clients.first(); c; c = c->next()) {
            Rect tab_rect(tabbar_rect.x + (i * (tab_width + _tab_gap)),
                          tabbar_rect.y, tab_width, tab_height);

            if (tab_rect.isPointInside(local_x, local_y)) {
                setActiveClient(c);
                break;
            }

            i++;
        }
    }
}


//FIXME bring in order

int ClientContainer::maxHeightInt()
{
	//FIXME
    return 0;
}

int ClientContainer::maxWidthInt()
{
    int min_width = minWidthInt();

    if (activeClient() && !isMinimized()) {
//         if (!isFixedSize()) {
            if (activeClient()->maxWidth()) {
                int max_width = (2 * _frame_width) + activeClient()->maxWidth();
                if (max_width > min_width)
                    return max_width;
                else
                    return min_width;
            } else
                return 0;
//         } else
//             return min_width;
    } else
        return min_width;
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

void ClientContainer::handleClientSizeHintChanged(Client *client)
{
    if (activeClient() == client)
        parent()->handleSizeHintsChanged(this);
    layout();
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
        layout();
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
        layout();
    else
        redraw();

    if (isEmpty() && _parent)
        _parent->setDirty(true);
}
#endif

void ClientContainer::draw(Canvas *canvas)
{
    if (isMinimized()) {
        if (isHorizontal())
            drawTabbar(canvas);
        else
            drawVerticalTabs(canvas);
    } else {
        switch(_mode) {
        case TABBED:
            drawTabbar(canvas);
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

    canvas->drawText(title.str().c_str(), tabbar_rect, 0xFFFFFF);

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

void ClientContainer::drawTab(Client *client, const Rect &rect, bool minimized, bool vertical, Canvas *canvas)
{
    uint32 fg = Colors::TAB_TEXT;
    uint32 bg = Colors::TAB;

    if (client->hasFocus()) {
        bg = Colors::TAB_FOCUSED;
        fg = Colors::TAB_FOCUSED_TEXT;
    } else if (hasFocus() && activeClient() == client) {
        bg = Colors::TAB_ACTIVE;
        fg = Colors::TAB_ACTIVE_TEXT;
    } else if (activeClient() == client) {
        bg = Colors::TAB_CURRENT;
        fg = Colors::TAB_CURRENT_TEXT;
    }

    canvas->fillRectangle(rect, bg);

    canvas->drawFrame(rect, fg);

    if (Icon *icon = client->icon()) {
        int icon_x = rect.x + _tab_inner_margin;
        int icon_y = rect.y + _tab_inner_margin;
        canvas->drawIcon(icon, icon_x, icon_y);
    }

    Rect text_rect(rect.x + _tab_inner_margin, rect.y + _tab_inner_margin,
                   rect.w - (2* _tab_inner_margin), rect.h - (2* _tab_inner_margin));

    if (Icon *icon = client->icon()) {
        if (vertical) {
            text_rect.y += (icon->width() + 5);
        } else {
            text_rect.x += (icon->width() + 5);
            text_rect.w -= (icon->width() + 5);
        }
    }

    if (vertical) {
        const std::string name = client->iconName().empty() ?
                                    client->name() : client->iconName();
        canvas->drawText(client->className(), text_rect, fg);
        text_rect.y += (maxTextHeight() + _tab_inner_margin);
        canvas->drawText(name, text_rect, fg);
    } else {
        const std::string text = (minimized && !client->iconName().empty()) ?
                                            client->iconName() : client->name();

        canvas->drawText(client->title(), text_rect, fg);
    }
}

void ClientContainer::drawVerticalTabs(Canvas *canvas)
{
    Rect bg_rect = _rect;
    bg_rect.setPos(0, 0);
    canvas->erase(bg_rect);

    int i = 0;
    for (Client *c = _clients.first(); c; c = c->next()) {
        Rect tab_rect(_frame_width, _frame_width + (i * (_vertical_tabbar_width + _tab_gap)),
                      _vertical_tabbar_width, _vertical_tabbar_width);

        drawTab(c, tab_rect, isMinimized(), true, canvas);

        i++;
    }
}

void ClientContainer::drawTabbar(Canvas *canvas)
{
    Rect bg_rect = _rect;
    bg_rect.setPos(0, 0);
    canvas->erase(bg_rect);

    Rect tabbar_rect;
    getTabbbarRect(tabbar_rect);

    //FIXME hackish
    Rect status_bar_rect = tabbar_rect;
    status_bar_rect.x = _frame_width;
    status_bar_rect.w = _status_bar_width;
    status_bar_rect.x += _tab_inner_margin;
    status_bar_rect.y += _tab_inner_margin;
    status_bar_rect.w += (2 * _tab_inner_margin);
    status_bar_rect.h += (2 * _tab_inner_margin);

    uint32 status_bar_fg = Colors::TAB_TEXT;

    canvas->drawText(isFixedSize() ? "-" : "<->", status_bar_rect, status_bar_fg);

    if (!_clients.count())
        return;

    int tab_width, tab_height;
    getTabSize(tab_width, tab_height);

    int i = 0;
    for(Client *c = _clients.first(); c; c = c->next()) {
        Rect tab_rect(tabbar_rect.x + (i * (tab_width + _tab_gap)),
                      tabbar_rect.y, tab_width, tab_height);

        drawTab(c, tab_rect, isMinimized(), false, canvas);

        i++;
    }
}

void ClientContainer::layout()
{
    if (!isMinimized()) {
        switch(_mode) {
        case TABBED:
            layoutTabbed();
            break;
        case STACKED:
            layoutStacked(0);
            break;
        }
    }
    redraw();
}

int ClientContainer::calcTabbarHeight()
{
    return maxTextHeight() + (2 * _tab_inner_margin);
}

void ClientContainer::getTabbbarRect(Rect &rect)
{
    int tabbar_x = _frame_width + _status_bar_width;
    int tabbar_y = _frame_width;
    int tabbar_w = _rect.w - _status_bar_width - (2 * _frame_width);
    int tabbar_h = calcTabbarHeight();

    rect.set(tabbar_x, tabbar_y, tabbar_w, tabbar_h);
}

void ClientContainer::getTabSize(int &tab_width, int &tab_height)
{
    if (_clients.count()) {
        Rect tabbar_rect;
        getTabbbarRect(tabbar_rect);

        int num_gaps = _clients.count() - 1;

        tab_width = (tabbar_rect.w - (num_gaps * _tab_gap)) / _clients.count();
        tab_height = tabbar_rect.h;
    } else {
        tab_width = 0;
        tab_height = 0;
    }
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
    debug;

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


#if 0
void ClientContainer::focusSibling(Direction where)
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

void ClientContainer::createSibling(Direction where)
{
    std::cout<<"ClientContainer::createSibling()\n";
    bool prepend = !isForwardDirection(where);
    if (orientationOfDirection(where) == _parent->orientation()) {
        _parent->addNewClientContainer(prepend);
    } else
        _parent->splitChild(this, prepend);
}
#endif

#if 0
ClientContainer *ClientContainer::createSiblingFor(Container *container, bool prepend_new_sibling)
{
    ClientContainer *new_sibling = 0;

    if (container->parent())
        new_sibling = container->parent()->addNewClientContainer(prepend_new_sibling);

    return new_sibling;
}
#endif
#if 0
ClientContainer *ClientContainer::getOrCreateSiblingFor(Container *container, bool get_prev)
{
    if (!get_prev && container->next())
        return container->next()->activeClientContainer();
    else if (get_prev && container->prev())
        return container->prev()->activeClientContainer();
    else
        return createSiblingFor(container, get_prev);
}

ClientContainer *ClientContainer::getSibling(bool get_prev)
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
            if (numMappedClients() > 1) // only create new direct sibling if container doesn't become empty
                target = getOrCreateSiblingFor(this, backward);
            else
                target = getSibling(backward);
        }

        else { // difficult case:
                 // if client container becomes empty -> use use sibling of parent container;
                 // else: 1. replace this with new parent container; 2. add this and new client container to parent created in step 1

            if (numMappedClients() <= 1) // cant't split - use use sibling of parent container
                target = getOrCreateSiblingFor(_parent, backward);
            else { // split this
//                 target = splitContainer(this, backward);
                target = _parent->splitChild(this, backward);

                if (!target) { // split failed - maximum hierarchy depth exceeded ?
                    target = getOrCreateSiblingFor(_parent, backward);
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
