#ifndef __FRONTEND_THEME_H__
#define __FRONTEND_THEME_H__

#include "rect.h"

class Canvas;
class Widget;
class ClientWrapper;

namespace Theme
{
    void drawWidget(Widget *widget, Canvas *canvas);
    void drawClientFrame(ClientWrapper *client, Canvas *canvas, const Rect &rect);
}

#endif // __THEME_H__
