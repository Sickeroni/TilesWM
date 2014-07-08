#ifndef __THEME_H__
#define __THEME_H__

#include "rect.h"

namespace Theme
{
    void calcClientFrameRect(bool has_decoration, int max_text_height, const Rect &client_rect, Rect &frame_rect);
    void calcClientClientRect(bool has_decoration, int max_text_height, const Rect &frame_rect, Rect &client_rect);
}

#endif // __THEME_H__
