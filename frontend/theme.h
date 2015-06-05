#ifndef __THEME_H__
#define __THEME_H__

#include "application.h"

class Canvas;
class Workspace;
class ThemeBackend;

namespace Theme
{
    void drawWorkspace(Workspace *workspace, Canvas *canvas);

    inline ThemeBackend *backend() {
        return Application::themeBackend();
    }
}

#endif // __THEME_H__
