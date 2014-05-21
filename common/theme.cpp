#include "theme.h"

#include "container_container.h"
#include "client_container.h"
#include "client.h"
#include "icon.h"
#include "canvas.h"
#include "colors.h"
#include "metrics.h"

#include <sstream>


namespace Theme {


struct ClientContainerSizesInternal
{
    int frame_width;
    int vertical_tabbar_width;
    int tab_inner_margin;
    int tab_gap;
    int status_bar_width;
};


ContainerContainerSizes _containerContainerSizes = {
    .child_frame_width = 10,
    .title_height = 10,
    .frame_width = 10
};

ClientContainerSizesInternal _clientContainerSizesInternal = {
    .frame_width = Metrics::CLIENT_CONTAINER_FRAME_MARGIN,
    .vertical_tabbar_width = 70,
    .tab_inner_margin = 5,
    .tab_gap = 2,
    .status_bar_width = 30
};

ClientContainerSizes _clientContainerSizes = {
    .min_contents_width = _clientContainerSizesInternal.vertical_tabbar_width,
    .frame_width = _clientContainerSizesInternal.frame_width
};


const ContainerContainerSizes &containerContainerSizes()
{
    return _containerContainerSizes;
}

const ClientContainerSizes &clientContainerSizes()
{
    return _clientContainerSizes;
}


void drawContainerContainer(ContainerContainer *container, Canvas *canvas)
{
    const ContainerContainerSizes &sizes = _containerContainerSizes;

    Rect bg_rect = container->rect();
    bg_rect.setPos(0, 0);
    canvas->erase(bg_rect);

    Rect title_rect(sizes.frame_width - 2, sizes.frame_width - 2,
                    container->width() - 4 - (2 * sizes.frame_width), sizes.title_height);

    std::stringstream title;

    if (container->isHorizontal())
        title<<"H";
    else
        title<<"V";

    if (container->isFixedSize())
        title<<"  - ";
    else
        title<<" <->";

    canvas->drawText(title.str().c_str(),
                     title_rect,
                     container->isActive() ?  Colors::CONTAINER_FOCUS : Colors::CONTAINER_BORDER);

    for (int i = 0; i < container->numElements(); i++) {
        Container *c = container->child(i);
        Rect frame_rect = c->rect();
        frame_rect.x -= 5;
        frame_rect.y -= 5;
        frame_rect.w += 10;
        frame_rect.h += 10;

        uint32 frame_color = c->isClientContainer() ? Colors::CLIENT_CONTAINER_BORDER:
                                                      Colors::CONTAINER_BORDER;

        canvas->drawFrame(frame_rect, frame_color);

        if (c->isActive()) {
            Rect focus_rect = frame_rect;
            focus_rect.x += 1;
            focus_rect.y += 1;
            focus_rect.w -= 2;
            focus_rect.h -= 2;

            uint32 color = c->isClientContainer() ? Colors::CLIENT_CONTAINER_FOCUS :
                                                    Colors::CONTAINER_FOCUS;

            canvas->drawFrame(focus_rect, color);
        }
    }
}


int calcTabbarHeight(ClientContainer *container) {
    return container->maxTextHeight() + (2 * _clientContainerSizesInternal.tab_inner_margin);
}

int calcVerticalTabbarHeight(ClientContainer *container) {
    return container->numElements() * Theme::calcTabbarHeight(container)
                + ((!container->numElements() ? 0 : container->numElements() - 1) * _clientContainerSizesInternal.tab_gap);
}

void getTabbbarRect(ClientContainer *container, Rect &rect)
{
    const ClientContainerSizesInternal &sizes = _clientContainerSizesInternal;

    int tabbar_x = sizes.frame_width + sizes.status_bar_width;
    int tabbar_y = sizes.frame_width;
    int tabbar_w = container->rect().w - sizes.status_bar_width - (2 * sizes.frame_width);
    int tabbar_h = calcTabbarHeight(container);

    rect.set(tabbar_x, tabbar_y, tabbar_w, tabbar_h);
}

void getTabSize(ClientContainer *container, int &tab_width, int &tab_height)
{
    if (container->numElements()) {
        Rect tabbar_rect;
        getTabbbarRect(container, tabbar_rect);

        int num_gaps = container->numElements() - 1;

        tab_width = (tabbar_rect.w - (num_gaps * _clientContainerSizesInternal.tab_gap)) / container->numElements();
        tab_height = tabbar_rect.h;
    } else {
        tab_width = 0;
        tab_height = 0;
    }
}

void drawTab(ClientContainer *container, Client *client, const Rect &rect, bool vertical, Canvas *canvas)
{
    const ClientContainerSizesInternal &sizes = _clientContainerSizesInternal;
    uint32 fg = Colors::TAB_TEXT;
    uint32 bg = Colors::TAB;

    if (client->hasFocus()) {
        bg = Colors::TAB_FOCUSED;
        fg = Colors::TAB_FOCUSED_TEXT;
    } else if (container->isActive() && container->activeClient() == client) {
        bg = Colors::TAB_ACTIVE;
        fg = Colors::TAB_ACTIVE_TEXT;
    } else if (container->activeClient() == client) {
        bg = Colors::TAB_CURRENT;
        fg = Colors::TAB_CURRENT_TEXT;
    }

    canvas->fillRectangle(rect, bg);

    canvas->drawFrame(rect, fg);

    if (Icon *icon = client->icon()) {
        int icon_x = rect.x + sizes.tab_inner_margin;
        int icon_y = rect.y + sizes.tab_inner_margin;
        canvas->drawIcon(icon, icon_x, icon_y);
    }

    Rect text_rect(rect.x + sizes.tab_inner_margin, rect.y + sizes.tab_inner_margin,
                   rect.w - (2* sizes.tab_inner_margin), rect.h - (2* sizes.tab_inner_margin));

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
        text_rect.y += (container->maxTextHeight() + sizes.tab_inner_margin);
        canvas->drawText(name, text_rect, fg);
    } else {
        const std::string text = (container->isMinimized() && !client->iconName().empty()) ?
                                            client->iconName() : client->name();

        canvas->drawText(client->title(), text_rect, fg);
    }
}

void drawTabbar(ClientContainer *container, Canvas *canvas)
{
    const ClientContainerSizesInternal &sizes = _clientContainerSizesInternal;

    Rect bg_rect = container->rect();
    bg_rect.setPos(0, 0);
    canvas->erase(bg_rect);

    Rect tabbar_rect;
    getTabbbarRect(container, tabbar_rect);

    //FIXME hackish
    Rect status_bar_rect = tabbar_rect;
    status_bar_rect.x = sizes.frame_width;
    status_bar_rect.w = sizes.status_bar_width;
    status_bar_rect.x += sizes.tab_inner_margin;
    status_bar_rect.y += sizes.tab_inner_margin;
    status_bar_rect.w += (2 * sizes.tab_inner_margin);
    status_bar_rect.h += (2 * sizes.tab_inner_margin);

    uint32 status_bar_fg = Colors::TAB_TEXT;

    canvas->drawText(container->isFixedSize() ? "-" : "<->", status_bar_rect, status_bar_fg);

    if (!container->numElements())
        return;

    int tab_width, tab_height;
    getTabSize(container, tab_width, tab_height);

    for(int i = 0; i < container->numElements(); i++) {
        Rect tab_rect(tabbar_rect.x + (i * (tab_width + sizes.tab_gap)),
                      tabbar_rect.y, tab_width, tab_height);

        drawTab(container, container->child(i), tab_rect, false, canvas);
    }
}

void drawVerticalTabs(ClientContainer *container, Canvas *canvas)
{
    const ClientContainerSizesInternal &sizes = _clientContainerSizesInternal;
    Rect bg_rect = container->rect();
    bg_rect.setPos(0, 0);
    canvas->erase(bg_rect);

    for (int i = 0; i < container->numElements(); i++) {
        Rect tab_rect(sizes.frame_width, sizes.frame_width + (i * (sizes.vertical_tabbar_width + sizes.tab_gap)),
                      sizes.vertical_tabbar_width, sizes.vertical_tabbar_width);

        drawTab(container, container->child(i), tab_rect, true, canvas);
    }
}

void drawClientContainer(ClientContainer *container, Canvas *canvas)
{
    if (container->isMinimized()) {
        if (container->isHorizontal())
            drawTabbar(container, canvas);
        else
            drawVerticalTabs(container, canvas);
    } else {
        drawTabbar(container, canvas);
    }
}


} // namespace Theme
