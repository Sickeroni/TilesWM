#include "frontend_theme.h"

#include "workspace.h"
#include "client.h"
#include "icon.h"
#include "canvas.h"
#include "colors.h"
#include "metrics.h"
#include "common.h"

#include <sstream>
#include <cmath>

using std::max;


namespace Theme {

void drawClientFrame(Client *client, Canvas *canvas)
{
    Rect frame_rect = client->rect();
    frame_rect.setPos(0, 0);

    assert(frame_rect.w && frame_rect.h);

    uint32_t frame_color = client->hasFocus() ? Colors::CLIENT_FOCUS : Colors::CLIENT_BORDER;

    canvas->begin();

    //FIXME
    canvas->erase(frame_rect);
//     canvas->fillRectangle(frame_rect, frame_color);

    Rect border_rect;
    border_rect.set(frame_rect.x+1, frame_rect.y+1, frame_rect.w-2, frame_rect.h-2);
    canvas->drawFrame(border_rect, frame_color);

    if (client->hasDecoration()) {
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
    }

    canvas->end();
}

void drawWorkspace(Workspace *workspace, Canvas *canvas)
{
    Rect rect = workspace->rect();
    rect.setPos(0, 0);
    canvas->begin();
    canvas->erase(rect);
    canvas->drawText("--------------- Workspace ---------------", rect, 0x00FF00);
    canvas->end();
}

void drawWidget(Widget *widget, Canvas *canvas)
{
    if (Workspace *workspace = widget->toWorkspace())
        drawWorkspace(workspace, canvas);
    else if (Client *client = widget->toClient())
        drawClientFrame(client, canvas);
    else
        debug<<"unknown widget type";
}


} // namespace Theme
