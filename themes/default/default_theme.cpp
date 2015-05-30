#include "default_theme.h"
#include "common.h"

namespace Metrics
{
    enum {
        CLIENT_CONTAINER_FRAME_MARGIN = 0,
        CLIENT_INNER_FRAME_MARGIN = 0,
        CLIENT_DECORATION_MARGIN = 4,
        CLIENT_TITLEBAR_INNER_MARGIN = 5,
        CLIENT_ICON_SIZE = 22,
        TITLEBAR_BOTTOM_MARGIN = 2,
        TABBAR_TAB_GAP = 2
    };
}

int DefaultTheme::titlebarHeight(int max_text_height) {
    return max_text_height + (2 * Metrics::CLIENT_TITLEBAR_INNER_MARGIN);
}

int DefaultTheme::tabbarHeight(const TabbarInfo &tabbar)
{
    int tab_height = tabHeight(tabbar);
    if (tabbar.is_vertical) {
        return tab_height * tabbar.num_elements
                    + Metrics::TABBAR_TAB_GAP * (tabbar.num_elements ? tabbar.num_elements - 1 : 0);
    } else
        return tabHeight(tabbar);
}

int DefaultTheme::tabHeight(const TabbarInfo &tabbar)
{
    if (tabbar.is_vertical) {
        int height = 2 * tabbar.max_text_height + 3 * Metrics::CLIENT_TITLEBAR_INNER_MARGIN;
        if (tabbar.icon_size)
            height += tabbar.icon_size + Metrics::CLIENT_TITLEBAR_INNER_MARGIN;
        return height;
    } else
        //FIXME icon_size
        return titlebarHeight(tabbar.max_text_height);
}

int DefaultTheme::titlebarBottomMargin() {
    return Metrics::TITLEBAR_BOTTOM_MARGIN;
}

int DefaultTheme::clientFrameMargin() {
    return Metrics::CLIENT_DECORATION_MARGIN;
}

int DefaultTheme::clientIconSize() {
    return Metrics::CLIENT_ICON_SIZE;
}

inline int horizontalTabWidth(int num_tabs, int tabbar_width)
{
    if (num_tabs) {
        int num_gaps = num_tabs - 1;
        return (tabbar_width - (num_gaps * Metrics::TABBAR_TAB_GAP)) / num_tabs;
    } else
        return 0;
}

void DefaultTheme::getTabSize(const TabbarInfo &tabbar, const Rect &tabbar_rect, int &width, int &height)
{
    width = tabbar.is_vertical ?
        tabbar_rect.w : horizontalTabWidth(tabbar.num_elements, tabbar_rect.w);
    height = tabHeight(tabbar);
}

inline void getTabPos(
        const int index,
        bool is_vertical,
        const Rect &tabbar_rect,
        Rect &rect)
{
    if (is_vertical) {
        rect.x = tabbar_rect.x;
        rect.y = tabbar_rect.y + (index * (rect.h + Metrics::TABBAR_TAB_GAP));
    } else {
        rect.x = tabbar_rect.x + (index * (rect.w + Metrics::TABBAR_TAB_GAP));
        rect.y = tabbar_rect.y;
    }
}

int DefaultTheme::getTabAt(int x, int y, const Rect &tabbar_rect, const TabbarInfo &tabbar)
{
    Rect tab_rect;
    getTabSize(tabbar, tabbar_rect, tab_rect.w, tab_rect.h);

    for(int i = 0; i < tabbar.num_elements; i++) {
        getTabPos(i, tabbar.is_vertical, tabbar_rect, tab_rect);
        if (tab_rect.isPointInside(x, y))
            return i;
    }

    return -1;
}

void DefaultTheme::drawTabbar(
        const TabbarInfo &tabbar,
        const std::vector<TabInfo> &tabs,
        int current_tab_index,
        bool current_tab_has_focus,
        const Rect &rect,
        Canvas *canvas)
{
    Rect tab_rect;
    getTabSize(tabbar, rect, tab_rect.w, tab_rect.h);

    for(size_t i = 0; i < tabs.size(); i++) {
        getTabPos(i, tabbar.is_vertical, rect, tab_rect);

        const TabInfo &tab = tabs[i];

        bool is_active = (i == current_tab_index);
        bool has_focus = (current_tab_has_focus && is_active);

        drawTab(tab, has_focus, is_active, tabbar, tab_rect, canvas);
    }
}

void DefaultTheme::drawTab(
        const TabInfo &tab,
        bool has_focus,
        bool is_active,
        const TabbarInfo &tabbar,
        const Rect &rect,
        Canvas *canvas)
{
    uint32_t fg = Colors::TAB_TEXT;
    uint32_t bg = Colors::TAB;
    uint32_t border = Colors::TAB_BORDER;

    if (has_focus) {
        bg = Colors::TAB_FOCUSED;
        fg = Colors::TAB_FOCUSED_TEXT;
        border = Colors::TAB_FOCUSED_BORDER;
    } else if (is_active) {
        bg = Colors::TAB_CURRENT;
        fg = Colors::TAB_CURRENT_TEXT;
    }

    drawTabOrTitlebar(tab.icon, tab.title1, tab.title2, tabbar.is_vertical, fg, bg, border, tabbar.max_text_height, rect, canvas);
}

void DefaultTheme::drawTitlebar(Icon *icon, const std::string &title, bool has_focus, const Rect &rect, Canvas *canvas) {
    uint32_t fg = has_focus ? Colors::TAB_FOCUSED_TEXT : Colors::TAB_TEXT;
    uint32_t bg = has_focus ? Colors::TAB_FOCUSED : Colors::TAB;
    uint32_t border = has_focus ? Colors::TAB_FOCUSED_BORDER : Colors::TAB_BORDER;
    drawTabOrTitlebar(icon, title, std::string(), false, fg, bg,border,  0, rect, canvas);
}

void DefaultTheme::drawClientFrame(bool has_focus, const Rect &rect, Canvas *canvas) {
    uint32_t frame_color = has_focus ? Colors::CLIENT_FOCUS : Colors::CLIENT_BORDER;

    //FIXME
    canvas->erase(rect);
//         canvas->fillRectangle(rect, 0x00FF00);

    Rect border_rect;
    border_rect.set(rect.x + 1, rect.y + 1, rect.w - 2, rect.h - 2);
    canvas->drawFrame(border_rect, frame_color);
}

void DefaultTheme::drawTabOrTitlebar(
        Icon *icon,
        const std::string &title1,
        const std::string &title2,
        bool is_vertical,
        uint32_t fg, uint32_t bg, uint32_t border_color,
        int max_text_height,
        const Rect &rect, Canvas *canvas)
{
    canvas->fillRectangle(rect, bg);
    canvas->drawFrame(rect, border_color);

    if (icon) {
        int icon_x = rect.x + Metrics::CLIENT_TITLEBAR_INNER_MARGIN;
        int icon_y = rect.y + Metrics::CLIENT_TITLEBAR_INNER_MARGIN;
        canvas->drawIcon(icon, icon_x, icon_y);
    }

    Rect title_rect = rect;
    title_rect.x += Metrics::CLIENT_TITLEBAR_INNER_MARGIN;
    title_rect.y += Metrics::CLIENT_TITLEBAR_INNER_MARGIN;
    title_rect.w -= (2 * Metrics::CLIENT_TITLEBAR_INNER_MARGIN);
    title_rect.h -= (2 * Metrics::CLIENT_TITLEBAR_INNER_MARGIN);
    if (icon) {
        if (is_vertical) {
            title_rect.y += (icon->width() + Metrics::CLIENT_TITLEBAR_INNER_MARGIN);
        } else {
            title_rect.x += (icon->width() + Metrics::CLIENT_TITLEBAR_INNER_MARGIN);
            title_rect.w -= (icon->width() + Metrics::CLIENT_TITLEBAR_INNER_MARGIN);
        }
    }
    if (is_vertical) {
        canvas->drawText(title1, title_rect, fg);
        title_rect.y += (max_text_height + Metrics::CLIENT_TITLEBAR_INNER_MARGIN);
        canvas->drawText(title2, title_rect, fg);
    } else
        canvas->drawText(title1, title_rect, fg);
}
