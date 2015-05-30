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

    virtual int titlebarHeight(int max_text_height) override;
    virtual int tabHeight(int max_text_height, bool is_vertical, int icon_size) override;
    virtual int titlebarBottomMargin() override;
    virtual int clientDecorationMargin() override;

    virtual void drawTabbar(
            const std::vector<TabInfo> &tabs,
            int current_tab_index,
            bool current_tab_has_focus,
            bool is_vertical,
            const Rect &rect,
            Canvas *canvas) override;

    virtual void drawTab(
            Icon *icon,
            const std::string &title1,
            const std::string &title2,
            bool has_focus,
            bool is_active,
            int max_text_height,
            const Rect &rect,
            Canvas *canvas) override;

    virtual void drawTitlebar(
            Icon *icon, const std::string &title,
            bool has_focus,
            const Rect &rect,
            Canvas *canvas) override;

    virtual void drawClientFrame(bool has_focus, const Rect &rect, Canvas *canvas) override;

private:
    void drawTabOrTitlebar(
            Icon *icon,
            const std::string &title1,
            const std::string &title2,
            bool is_vertical,
            uint32_t fg, uint32_t bg, uint32_t border_color,
            int max_text_height,
            const Rect &rect, Canvas *canvas);
};

#endif
