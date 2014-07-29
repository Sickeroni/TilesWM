#include "theme.h"
#include "workspace.h"
#include "icon.h"
#include "canvas.h"

namespace Theme
{


void drawWorkspace(Workspace *workspace, Canvas *canvas)
{
    Rect rect = workspace->rect();
    rect.setPos(0, 0);
    canvas->begin();

    if (workspace->background())
        canvas->drawIcon(workspace->background(), 0, 0);
    else
        canvas->erase(rect);

    canvas->end();
}

void drawWidget(Widget *widget, Canvas *canvas)
{
    if (Workspace *workspace = widget->toWorkspace())
        drawWorkspace(workspace, canvas);
    else
        debug<<"unknown widget type";
}


} // namespace Theme
