#include "mode_default.h"

#include "window_manager_default.h"

ModeDefault::ModeDefault() : Mode("tiling.default")
{
    WindowManagerDefault::createActions(_actions);
}

WindowManager *ModeDefault::createWindowManager(Workspace *workspace)
{
    return new WindowManagerDefault(workspace, this);
}
