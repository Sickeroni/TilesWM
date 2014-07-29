#include "floating_theme.h"
#include "client_wrapper.h"
#include "icon.h"
#include "canvas.h"
#include "colors.h"
#include "metrics.h"
#include "common.h"

namespace Theme
{


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

void drawClientFrame(ClientWrapper *client, Canvas *canvas, const Rect &rect)
{
    Rect frame_rect = rect;
    frame_rect.setPos(0, 0);

    assert(frame_rect.w && frame_rect.h);

    uint32_t frame_color = client->hasFocus() ? Colors::CLIENT_FOCUS : Colors::CLIENT_BORDER;

    canvas->begin();

    //FIXME
    canvas->erase(frame_rect);
//     canvas->fillRectangle(frame_rect, 0x00FF00);

    Rect border_rect;
    border_rect.set(frame_rect.x+1, frame_rect.y+1, frame_rect.w-2, frame_rect.h-2);
    canvas->drawFrame(border_rect, frame_color);

//     if (client->hasDecoration()) {
        //FIXME duplicated in calcFrameMargins() / drawTab()
        int frame_margin = Metrics::CLIENT_INNER_FRAME_MARGIN;
        int titlebar_height = 0;

        frame_margin += Metrics::CLIENT_DECORATION_MARGIN;
        titlebar_height +=
            (client->maxTextHeight() + (2 * Metrics::CLIENT_TITLEBAR_INNER_MARGIN));

        uint32_t title_fg = client->hasFocus() ? Colors::TAB_FOCUSED_TEXT : Colors::TAB_TEXT;
        uint32_t title_bg = client->hasFocus() ? Colors::TAB_FOCUSED : Colors::TAB;

        Rect titlebar_rect(frame_margin, frame_margin,
                           frame_rect.w - (2 * frame_margin), titlebar_height);

        canvas->fillRectangle(titlebar_rect, title_bg);
        canvas->drawFrame(titlebar_rect, title_fg);

        if (client->icon()) {
            int icon_x = titlebar_rect.x + Metrics::CLIENT_TITLEBAR_INNER_MARGIN;
            int icon_y = titlebar_rect.y + Metrics::CLIENT_TITLEBAR_INNER_MARGIN;
            canvas->drawIcon(client->icon(), icon_x, icon_y);
        }

        Rect title_rect = titlebar_rect;
        title_rect.x += Metrics::CLIENT_TITLEBAR_INNER_MARGIN;
        title_rect.y += Metrics::CLIENT_TITLEBAR_INNER_MARGIN;
        title_rect.w -= (2 * Metrics::CLIENT_TITLEBAR_INNER_MARGIN);
        title_rect.h -= (2* Metrics::CLIENT_TITLEBAR_INNER_MARGIN);
        if (client->icon()) {
            title_rect.x += (client->icon()->width() + 5);
            title_rect.w -= (client->icon()->width() + 5);
        }
        canvas->drawText(client->name(), title_rect, title_fg);
//     }

    canvas->end();
}


} // namespace Theme
