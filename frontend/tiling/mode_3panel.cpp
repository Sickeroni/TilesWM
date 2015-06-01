#include "mode_3panel.h"

#include "window_manager_3panel.h"

Mode3Panel::Mode3Panel() : Mode("tiling.3panel")
{
    WindowManager3Panel::createActions(_actions);
}

WindowManager *Mode3Panel::createWindowManager(Workspace *workspace) {
    return new WindowManager3Panel(workspace, this);
}
