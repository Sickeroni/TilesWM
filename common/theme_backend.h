#ifndef THEME_BACKEND_H
#define THEME_BACKEND_H

#include "rect.h"

#include <string>

class Canvas;
class Icon;

class ThemeBackend
{
public:
    virtual ~ThemeBackend() {}

    virtual int titlebarHeight(int max_text_height) = 0;
    virtual int tabHeight(int max_text_height, bool is_vertical, int icon_size) = 0;
    virtual int titlebarBottomMargin() = 0;
    virtual int clientDecorationMargin() = 0;

    virtual void drawTab(
            Icon *icon,
            const std::string &title1,
            const std::string &title2,
            bool has_focus,
            bool is_active,
            int max_text_height,
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
