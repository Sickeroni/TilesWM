#ifndef DEFAULT_THEME_H
#define DEFAULT_THEME_H

#include "theme_backend.h"
#include "colors.h"
#include "canvas.h"
#include "icon.h"

class DefaultTheme : public ThemeBackend
{
public:
    virtual ~DefaultTheme() {}

    virtual int tabbarHeight(const TabbarInfo &tabbar) override;
    virtual int getTabAt(int x, int y, const Rect &tabbar_rect, const TabbarInfo &tabbar) override;

    virtual int titlebarHeight(int max_text_height) override;
    virtual int titlebarBottomMargin() override;
    virtual int clientFrameMargin() override;
    virtual int clientIconSize() override;

    virtual void drawTabbar(
            const TabbarInfo &tabbar,
            const std::vector<TabInfo> &tabs,
            int current_tab_index,
            bool current_tab_has_focus,
            const Rect &rect,
            Canvas *canvas) override;

    virtual void drawTitlebar(
            Icon *icon, const std::string &title,
            bool has_focus,
            const Rect &rect,
            Canvas *canvas) override;

    virtual void drawClientFrame(bool has_focus, const Rect &rect, Canvas *canvas) override;

private:
    int tabHeight(const TabbarInfo &tabbar);
    void getTabSize(const TabbarInfo &tabbar, const Rect &tabbar_rect, int &width, int &height);

    void drawTabOrTitlebar(
            Icon *icon,
            const std::string &title1,
            const std::string &title2,
            bool is_vertical,
            uint32_t fg, uint32_t bg, uint32_t border_color,
            int max_text_height,
            const Rect &rect, Canvas *canvas);

    void drawTab(
            const TabInfo &tab,
            bool has_focus,
            bool is_active,
            const TabbarInfo &tabbar,
            const Rect &rect,
            Canvas *canvas);
};

#endif
