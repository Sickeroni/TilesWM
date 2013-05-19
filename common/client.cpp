#include "client.h"

#include "client_container.h"
#include "canvas.h"
#include "icon.h"
#include "colors.h"
#include "types.h"

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


Client::Client(bool is_mapped) :
    _is_mapped(is_mapped),
    _has_focus(false),
    _name("<no name>"),
    _min_width(0),
    _min_height(0),
    _max_width(0),
    _max_height(0),
    _container(0)
{
}

Client::~Client()
{
    std::cout<<"Client::~Client()\n";
    assert(!_container);
}
void Client::setContainer(ClientContainer *c)
{
    _container = c;
}

void Client::drawFrame(Canvas *canvas)
{
    Rect frame_rect = frameRect();
    frame_rect.setPos(0, 0);

    uint32 frame_color = _has_focus ? Colors::CLIENT_FOCUS : Colors::CLIENT_BORDER;

    //FIXME
    canvas->erase(frame_rect);
//     canvas->fillRectangle(frame_rect, frame_color);

    Rect border_rect;
    border_rect.set(frame_rect.x+1, frame_rect.y+1, frame_rect.w-2, frame_rect.h-2);
    canvas->drawFrame(border_rect, frame_color);


    //FIXME duplicated in calcFrameMargins() / ClientContainer::drawTab()
    int frame_margin = Metrics::CLIENT_INNER_FRAME_MARGIN;
    int titlebar_height = 0;
    if (hasDecoration()) {
        frame_margin += Metrics::CLIENT_DECORATION_MARGIN;
        titlebar_height +=
            (maxTextHeight() + (2 * Metrics::CLIENT_TITLEBAR_INNER_MARGIN));

        uint32 title_fg = _has_focus ? Colors::TAB_FOCUSED_TEXT : Colors::TAB_TEXT;
        uint32 title_bg = _has_focus ? Colors::TAB_FOCUSED : Colors::TAB;

        Rect titlebar_rect(frame_margin, frame_margin,
                           frame_rect.w - (2 * frame_margin), titlebar_height);

        canvas->fillRectangle(titlebar_rect, title_bg);
        canvas->drawFrame(titlebar_rect, title_fg);

        if (icon()) {
            int icon_x = titlebar_rect.x + Metrics::CLIENT_TITLEBAR_INNER_MARGIN;
            int icon_y = titlebar_rect.y + Metrics::CLIENT_TITLEBAR_INNER_MARGIN;
            canvas->drawIcon(icon(), icon_x, icon_y);
        }

        Rect title_rect = titlebar_rect;
        title_rect.x += Metrics::CLIENT_TITLEBAR_INNER_MARGIN;
        title_rect.y += Metrics::CLIENT_TITLEBAR_INNER_MARGIN;
        title_rect.w -= (2 * Metrics::CLIENT_TITLEBAR_INNER_MARGIN);
        title_rect.h -= (2* Metrics::CLIENT_TITLEBAR_INNER_MARGIN);
        if (icon()) {
            title_rect.x += (icon()->width() + 5);
            title_rect.w -= (icon()->width() + 5);
        }
        canvas->drawText(name(), title_rect, title_fg);
    }
}

void Client::calcFrameMargins(int &side, int &top, int &bottom)
{
    int frame_margin = Metrics::CLIENT_INNER_FRAME_MARGIN;
    int titlebar_height = 0;

    if (hasDecoration()) {
        frame_margin += Metrics::CLIENT_DECORATION_MARGIN;
        titlebar_height +=
            (maxTextHeight() + (2 * Metrics::CLIENT_TITLEBAR_INNER_MARGIN));
    }

    side = frame_margin;
    top = frame_margin + titlebar_height + _titlebar_gap;
    bottom = frame_margin;
}

void Client::calcFrameRect(const Rect &client_rect, Rect &frame_rect)
{
    int side_margin, top_margin, bottom_margin;
    calcFrameMargins(side_margin, top_margin, bottom_margin);

    frame_rect.set(client_rect);

    frame_rect.x -= side_margin;
    frame_rect.y -= top_margin;
    frame_rect.w += (2 * side_margin);
    frame_rect.h += (top_margin + bottom_margin);
}

void Client::calcClientRect(const Rect &frame_rect, Rect &client_rect)
{
    int side_margin, top_margin, bottom_margin;
    calcFrameMargins(side_margin, top_margin, bottom_margin);

    client_rect.set(frame_rect);

    client_rect.x = side_margin;
    client_rect.y = top_margin;
    client_rect.w -= (2 * side_margin);
    client_rect.h -= (top_margin + bottom_margin);
}
