#ifndef __FRONTEND_THEME_H__
#define __FRONTEND_THEME_H__

#include "rect.h"

class Canvas;
class Widget;

namespace Theme
{
    void drawWidget(Widget *widget, Canvas *canvas);
}

#endif // __THEME_H__
