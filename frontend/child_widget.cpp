#include "child_widget.h"

#include "widget_backend.h"
#include "common.h"

Workspace *ChildWidget::workspace()
{
    Workspace *w = 0;

    if (_parent) {
        if (ChildWidget *parent_child_widget = _parent->toChildWidget())
            w = parent_child_widget->workspace();
        else
            w = _parent->toWorkspace();
    }

    return w;
}

void ChildWidget::reparent(Widget *parent, WidgetBackend *parent_backend)
{
    setMapped(false);

    _rect.setPos(0, 0);

    _backend->reparent(parent_backend);
    _parent = parent;
}
