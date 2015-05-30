#ifndef THEME_BACKEND_H
#define THEME_BACKEND_H

#include "rect.h"

#include <string>
#include <vector>

class Canvas;
class Icon;

class ThemeBackend
{
public:
    struct TabInfo {
        const std::string &title1;
        const std::string &title2;
        Icon *icon;

        TabInfo(const std::string &title1, const std::string &title2, Icon *icon) :
            title1(title1), title2(title2), icon(icon) {}
    };
    struct TabbarInfo {
        int num_elements = 0;
        int max_text_height = 0;
        int icon_size = 0;
        bool is_vertical = false;
    };

    virtual ~ThemeBackend() {}

    virtual int tabbarHeight(const TabbarInfo &tabbar) = 0;
    virtual int getTabAt(int x, int y, const Rect &tabbar_rect, const TabbarInfo &tabbar) = 0;

    virtual int titlebarHeight(int max_text_height) = 0;
    virtual int titlebarBottomMargin() = 0;
    virtual int clientFrameMargin() = 0;
    virtual int clientIconSize() = 0;

    virtual void drawTabbar(
            const TabbarInfo &tabbar,
            const std::vector<TabInfo> &tabs,
            int current_tab_index,
            bool current_tab_has_focus,
            const Rect &rect,
            Canvas *canvas) = 0;

    virtual void drawTitlebar(
            Icon *icon,
            const std::string &title,
            bool has_focus,
            const Rect &rect,
            Canvas *canvas) = 0;

    virtual void drawClientFrame(bool has_focus, const Rect &rect, Canvas *canvas) = 0;
};

#endif
