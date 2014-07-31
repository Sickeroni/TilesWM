#include "client_container_theme.h"
#include "client_container.h"
#include "theme.h"
#include "theme_backend.h"
#include "client_wrapper.h"
#include "icon.h"
#include "canvas.h"
#include "colors.h"
#include "common.h"

#include <sstream>
#include <cmath>


using std::max;

namespace Theme {


struct ClientContainerSizesInternal
{
    int frame_width;
    int vertical_tabbar_width;
    int statusbar_inner_margin;
    int tab_gap;
    int status_bar_width;
};


const ClientContainerSizesInternal _clientContainerSizesInternal = {
    .frame_width = 0,
    .vertical_tabbar_width = 70,
    .statusbar_inner_margin = 5,
    .tab_gap = 2,
    .status_bar_width = 30
};

const ClientContainerSizes _clientContainerSizes = {
    .min_contents_width = _clientContainerSizesInternal.vertical_tabbar_width,
    .frame_width = _clientContainerSizesInternal.frame_width
};



const ClientContainerSizes &clientContainerSizes()
{
    return _clientContainerSizes;
}

void getTabbbarRect(ClientContainer *container, Rect &rect)
{
    const ClientContainerSizesInternal &sizes = _clientContainerSizesInternal;

    if (container->isMinimized() && container->isVertical()) {
        int tabbar_x = sizes.frame_width;
        int tabbar_y = sizes.frame_width;
        int tabbar_w = container->rect().w - (2 * sizes.frame_width);
        int tabbar_h = container->rect().h - (2 * sizes.frame_width);

        rect.set(tabbar_x, tabbar_y, tabbar_w, tabbar_h);
    } else {
        int tabbar_x = sizes.frame_width + sizes.status_bar_width;
        int tabbar_y = sizes.frame_width;
        int tabbar_w = container->rect().w - sizes.status_bar_width - (2 * sizes.frame_width);
        int tabbar_h = calcTabbarHeight(container);

        rect.set(tabbar_x, tabbar_y, tabbar_w, tabbar_h);
    }
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

void getHorizontalTabRect(
        const int tab_width,
        const int tab_height,
        const Rect &tabbar_rect,
        const int index,
        Rect &rect)
{
    const ClientContainerSizesInternal &sizes = _clientContainerSizesInternal;

    rect.set(
        tabbar_rect.x + (index * (tab_width + sizes.tab_gap)),
        tabbar_rect.y,
        tab_width,
        tab_height);
}

void getVerticalTabRect(
        const int index,
        Rect &rect)
{
    const ClientContainerSizesInternal &sizes = _clientContainerSizesInternal;

    rect.set(
        sizes.frame_width,
        sizes.frame_width + (index * (sizes.vertical_tabbar_width + sizes.tab_gap)),
        sizes.vertical_tabbar_width,
        sizes.vertical_tabbar_width);
}

int getTabAt(int x, int y, ClientContainer *container)
{
    Rect tabbar_rect;
    getTabbbarRect(container, tabbar_rect);

    printvar(tabbar_rect.y);
    printvar(tabbar_rect.h);

    if (tabbar_rect.isPointInside(x, y)) {
        if (container->isMinimized() && container->isVertical()) {
            for(int i = 0; i < container->numElements(); i++) {
                Rect tab_rect;
                getVerticalTabRect(i, tab_rect);

                if (tab_rect.isPointInside(x, y))
                    return i;
            }
        } else {
            int tab_width, tab_height;
            getTabSize(container, tab_width, tab_height);

            for(int i = 0; i < container->numElements(); i++) {
                Rect tab_rect;
                getHorizontalTabRect(tab_width, tab_height, tabbar_rect, i, tab_rect);

                if (tab_rect.isPointInside(x, y))
                    return i;
            }
        }
    }

    return Container::INVALID_INDEX;
}

int calcTabbarHeight(ClientContainer *container) {
    const int icon_size = 22; //FIXME
    return backend()->tabHeight(container->maxTextHeight(), false, icon_size);
}

int calcVerticalTabbarHeight(ClientContainer *container) {
    const int icon_size = 22; //FIXME
    return (container->numElements() * backend()->tabHeight(container->maxTextHeight(), true, icon_size))
                + ((!container->numElements() ? 0 : container->numElements() - 1) * _clientContainerSizesInternal.tab_gap);
}


void getClientContainerClientRect(ClientContainer *container,  Rect &client_rect)
{
    const ClientContainerSizesInternal &sizes = _clientContainerSizesInternal;
    int tabbar_height = calcTabbarHeight(container);

    static const int gap = backend()->titlebarBottomMargin();

    client_rect.x = sizes.frame_width;
    client_rect.y = sizes.frame_width + tabbar_height + gap;
    client_rect.w = container->width() - (2 * sizes.frame_width);
    client_rect.h = container->height() - ((2 * sizes.frame_width) + tabbar_height + gap);

    client_rect.w = max(0, client_rect.w);
    client_rect.h = max(0, client_rect.h);
}

void drawTab(ClientContainer *container, ClientWrapper *client, const Rect &rect, bool vertical, Canvas *canvas)
{
    std::string title1, title2;
    if (vertical) {
        title1 = client->className();
        title2 = client->iconName().empty() ? client->name() : client->iconName();
    } else
        title1 = client->title();

    backend()->drawTab(client->icon(),
        title1,
        title2,
        client->hasFocus(),
        container->activeClient() == client,
        client->maxTextHeight(),
        rect,
        canvas);
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
    status_bar_rect.x += sizes.statusbar_inner_margin;
    status_bar_rect.y += sizes.statusbar_inner_margin;
    status_bar_rect.w += (2 * sizes.statusbar_inner_margin);
    status_bar_rect.h += (2 * sizes.statusbar_inner_margin);

    uint32_t status_bar_fg = Colors::TAB_TEXT;

    canvas->drawText(container->isFixedSize() ? "-" : "<->", status_bar_rect, status_bar_fg);

    if (!container->numElements())
        return;

    int tab_width, tab_height;
    getTabSize(container, tab_width, tab_height);

    for(int i = 0; i < container->numElements(); i++) {
        Rect tab_rect;
        getHorizontalTabRect(tab_width, tab_height, tabbar_rect, i, tab_rect);

        drawTab(container, container->child(i), tab_rect, false, canvas);
    }
}

void drawVerticalTabs(ClientContainer *container, Canvas *canvas)
{
    Rect bg_rect = container->rect();
    bg_rect.setPos(0, 0);
    canvas->erase(bg_rect);

    Rect tabbar_rect;
    getTabbbarRect(container, tabbar_rect);

    for (int i = 0; i < container->numElements(); i++) {
        Rect tab_rect;
        getVerticalTabRect(i, tab_rect);

        drawTab(container, container->child(i), tab_rect, true, canvas);
    }
}

void drawClientContainer(ClientContainer *container, Canvas *canvas)
{
    if (container->isMinimized() && container->isVertical())
        drawVerticalTabs(container, canvas);
    else
        drawTabbar(container, canvas);
}


} // namespace Theme
