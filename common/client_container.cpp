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

    if (_active_client && !_active_client->hasFocus() && isActive())
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

#if 1
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

#if 1
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

//FIXME
#if 0
    if (isEmpty() && _parent)
        _parent->setDirty(true);
#endif
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
        drawTabbar(canvas);
    }
}


void ClientContainer::drawTab(Client *client, const Rect &rect, bool minimized, bool vertical, Canvas *canvas)
{
    uint32 fg = Colors::TAB_TEXT;
    uint32 bg = Colors::TAB;

    if (client->hasFocus()) {
        bg = Colors::TAB_FOCUSED;
        fg = Colors::TAB_FOCUSED_TEXT;
    } else if (isActive() && activeClient() == client) {
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
        Rect client_rect;
        getClientRect(client_rect);

        if (activeClient())
            activeClient()->raise();
        for(Client *c = _clients.first(); c; c = c->next())
            c->setRect(client_rect);
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
