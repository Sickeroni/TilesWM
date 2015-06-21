#include "theme.h"
#include "workspace.h"
#include "icon.h"
#include "canvas.h"

namespace Theme
{


void drawWorkspace(Widget *workspace, Icon *background, Canvas *canvas)
{
    Rect rect = workspace->rect();
    rect.setPos(0, 0);
    canvas->begin();

    if (background)
        canvas->drawIcon(background, 0, 0);
    else
        canvas->erase(rect);

    canvas->end();
}

} // namespace Theme
