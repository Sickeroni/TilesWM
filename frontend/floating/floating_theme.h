#ifndef FLOATING_THEME_H
#define FLOATING_THEME_H

#include "rect.h"

class Canvas;
class Widget;
class ClientWrapper;

namespace Theme
{
    void calcClientFrameRect(bool has_decoration, int max_text_height, const Rect &client_rect, Rect &frame_rect);
    void calcClientClientRect(bool has_decoration, int max_text_height, const Rect &frame_rect, Rect &client_rect);
    void drawClientFrame(ClientWrapper *client, Canvas *canvas, const Rect &rect);
}

#endif // FLOATING_THEME_H
