#include "default_theme.h"
#include "common.h"

namespace Metrics
{
    enum {
        CLIENT_CONTAINER_FRAME_MARGIN = 0,
        CLIENT_INNER_FRAME_MARGIN = 0,
        CLIENT_DECORATION_MARGIN = 4,
        CLIENT_TITLEBAR_INNER_MARGIN = 5,
        TABBAR_TAB_GAP = 2
    };
}

int DefaultTheme::titlebarHeight(int max_text_height) {
    return max_text_height + (2 * Metrics::CLIENT_TITLEBAR_INNER_MARGIN);
}

int DefaultTheme::tabHeight(int max_text_height, bool is_vertical, int icon_size) {
    if (is_vertical) {
        int height = 2 * max_text_height + 3 * Metrics::CLIENT_TITLEBAR_INNER_MARGIN;
        if (icon_size)
            height += icon_size + Metrics::CLIENT_TITLEBAR_INNER_MARGIN;
        return height;
    } else
        //FIXME icon_size
        return titlebarHeight(max_text_height);
}

int DefaultTheme::titlebarBottomMargin() {
    return 2;
}

int DefaultTheme::clientDecorationMargin() {
    return Metrics::CLIENT_DECORATION_MARGIN;
}


inline void getTabSize(int num_tabs, const Rect &tabbar_rect, int &tab_width, int &tab_height)
{
    if (num_tabs) {
        int num_gaps = num_tabs - 1;

        tab_width = (tabbar_rect.w - (num_gaps * Metrics::TABBAR_TAB_GAP)) / num_tabs;
        tab_height = tabbar_rect.h;
    } else {
        tab_width = 0;
        tab_height = 0;
    }
}

inline void getHorizontalTabRect(
        const int tab_width,
        const int tab_height,
        const Rect &tabbar_rect,
        const int index,
        Rect &rect)
{
    rect.set(
        tabbar_rect.x + (index * (tab_width + Metrics::TABBAR_TAB_GAP)),
        tabbar_rect.y,
        tab_width,
        tab_height);
}



void DefaultTheme::drawTabbar(
        const std::vector<TabInfo> &tabs,
        int current_tab_index,
        bool current_tab_has_focus,
        bool /*is_vertical*/,
        const Rect &rect,
        Canvas *canvas)
{
    int tab_width, tab_height;
    getTabSize(tabs.size(), rect, tab_width, tab_height);

    int max_text_height = canvas->maxTextHeight();

    for(size_t i = 0; i < tabs.size(); i++) {
        Rect tab_rect;
        getHorizontalTabRect(tab_width, tab_height, rect, i, tab_rect);

        const TabInfo &tab = tabs[i];

        bool is_active = (i == current_tab_index);
        bool has_focus = (current_tab_has_focus &&  is_active);

        drawTab(tab.icon, tab.title1, tab.title2, has_focus, is_active, max_text_height, rect, canvas);
    }

}

void DefaultTheme::drawTab(
        Icon *icon,
        const std::string &title1,
        const std::string &title2,
        bool has_focus,
        bool is_active,
        int max_text_height,
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

    drawTabOrTitlebar(icon, title1, title2, false, fg, bg, border, max_text_height, rect, canvas);
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
