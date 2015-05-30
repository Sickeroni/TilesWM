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
    int status_bar_width;
};


const ClientContainerSizesInternal _clientContainerSizesInternal = {
    .frame_width = 0,
    .vertical_tabbar_width = 70,
    .statusbar_inner_margin = 5,
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

void fillTabbarInfo(ClientContainer *container, ThemeBackend::TabbarInfo &info)
{
    info.num_elements = container->numElements();
    info.is_vertical = container->isMinimized() && container->isVertical();
    info.max_text_height = container->maxTextHeight();
    info.icon_size = Application::themeBackend()->clientIconSize();
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
    rect.w = max(0, rect.w);
    rect.h = max(0, rect.h);
    assert(rect.isValid());
}

int getTabAt(int x, int y, ClientContainer *container)
{
    Rect tabbar_rect;
    getTabbbarRect(container, tabbar_rect);

    printvar(tabbar_rect.y);
    printvar(tabbar_rect.h);

    if (tabbar_rect.isPointInside(x, y)) {
        ThemeBackend::TabbarInfo info;
        fillTabbarInfo(container, info);

        return backend()->getTabAt(x, y, tabbar_rect, info);
    } else
        return -1;
}

int calcTabbarHeight(ClientContainer *container) {
    ThemeBackend::TabbarInfo info;
    fillTabbarInfo(container, info);

    return backend()->tabbarHeight(info);
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

void drawClientContainer(ClientContainer *container, Canvas *canvas)
{
    const ClientContainerSizesInternal &sizes = _clientContainerSizesInternal;

    Rect bg_rect = container->rect();
    if (bg_rect.isEmpty())
        return;

    bg_rect.setPos(0, 0);
    canvas->erase(bg_rect); //FIXME

    Rect tabbar_rect;
    getTabbbarRect(container, tabbar_rect);

    ThemeBackend::TabbarInfo info;
    fillTabbarInfo(container, info);

    if (!info.is_vertical) {
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
    }

    printvar(container->numElements());

    if (!container->numElements())
        return;

    std::vector<ThemeBackend::TabInfo> tabs;
    tabs.reserve(container->numElements());
    std::string empty_string;
    for (Container::Index i = 0; i < container->numElements(); i++) {
        ClientWrapper *client = container->child(i);
        tabs.push_back(ThemeBackend::TabInfo(client->title(), empty_string, client->icon()));
    }

    int current_tab_index = (container->activeChildIndex() != Container::INVALID_INDEX) ?
        int_cast<int>(make_signed<long>(container->activeChildIndex())) : -1;

    assert(tabbar_rect.isValid());

    if (!tabbar_rect.isEmpty()) {
        backend()->drawTabbar(
                info,
                tabs,
                current_tab_index,
                container->activeClient()->hasFocus(),
                tabbar_rect,
                canvas);
    }
}


} // namespace Theme
