#ifndef __THEME_H__
#define __THEME_H__

#include "application.h"

class Canvas;
class Widget;
class ThemeBackend;

namespace Theme
{
    void drawWidget(Widget *widget, Canvas *canvas);

    inline ThemeBackend *backend() {
        return Application::themeBackend();
    }
}

#endif // __THEME_H__
