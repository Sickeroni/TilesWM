#include "floating_theme.h"
#include "theme_backend.h"
#include "client_wrapper.h"
#include "theme.h"
#include "icon.h"
#include "canvas.h"
#include "common.h"

namespace Theme
{


void calcClientFrameMargins(bool has_decoration, int max_text_height, int &side, int &top, int &bottom)
{
    int frame_margin = 0;
    int titlebar_height = 0;

    if (has_decoration) {
        frame_margin += backend()->clientDecorationMargin();
        titlebar_height +=
            (backend()->titlebarHeight(max_text_height) + backend()->titlebarBottomMargin());
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

void drawClientFrame(ClientWrapper *client, Canvas *canvas, const Rect &rect)
{
    Rect frame_rect = rect;
    frame_rect.setPos(0, 0);

    assert(frame_rect.w && frame_rect.h);

    canvas->begin();

    backend()->drawClientFrame(client->hasFocus(), frame_rect, canvas);

    //FIXME - add parameter has_decoration
//     if (client->hasDecoration()) {
        int frame_margin = backend()->clientDecorationMargin();
        int titlebar_height = backend()->titlebarHeight(client->maxTextHeight());

        Rect titlebar_rect(frame_margin, frame_margin,
                           frame_rect.w - (2 * frame_margin), titlebar_height);

        backend()->drawTitlebar(client->icon(), client->title(), client->hasFocus(), titlebar_rect, canvas);
//     }

    canvas->end();
}


} // namespace Theme
