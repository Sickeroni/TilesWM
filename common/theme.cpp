#include "theme.h"
#include "metrics.h"

namespace Theme {


void calcClientFrameMargins(bool has_decoration, int max_text_height, int &side, int &top, int &bottom)
{
    static const int titlebar_gap = 4; //FIXME
    int frame_margin = Metrics::CLIENT_INNER_FRAME_MARGIN;
    int titlebar_height = 0;

    if (has_decoration) {
        frame_margin += Metrics::CLIENT_DECORATION_MARGIN;
        titlebar_height +=
            (max_text_height + titlebar_gap + (2 * Metrics::CLIENT_TITLEBAR_INNER_MARGIN));
    }

    side = frame_margin;
    top = frame_margin + titlebar_height;
    bottom = frame_margin;
}

void calcClientFrameRect(bool has_decoration, int max_text_height, const Rect &client_rect, Rect &frame_rect)
{
    int side_margin, top_margin, bottom_margin;
    calcClientFrameMargins(has_decoration, max_text_height, side_margin, top_margin, bottom_margin);

    frame_rect.set(client_rect);

    frame_rect.x -= side_margin;
    frame_rect.y -= top_margin;
    frame_rect.w += (2 * side_margin);
    frame_rect.h += (top_margin + bottom_margin);
}

void calcClientClientRect(bool has_decoration, int max_text_height, const Rect &frame_rect, Rect &client_rect)
{
    int side_margin, top_margin, bottom_margin;
    calcClientFrameMargins(has_decoration, max_text_height, side_margin, top_margin, bottom_margin);

    client_rect.set(frame_rect);

    client_rect.x = side_margin;
    client_rect.y = top_margin;
    client_rect.w -= (2 * side_margin);
    client_rect.h -= (top_margin + bottom_margin);
}


} // namespace Theme
