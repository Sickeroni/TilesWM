#ifndef __THEME_H__
#define __THEME_H__

#include "application.h"

class Canvas;
class Icon;
class ThemeBackend;

namespace Theme
{
    void drawWorkspace(Widget *workspace, Icon *background, Canvas *canvas);

    inline ThemeBackend *backend() {
        return Application::themeBackend();
    }
}

#endif // __THEME_H__
