#include "mode_default.h"

#include "window_manager_default.h"

ModeDefault::ModeDefault() : Mode("default")
{
}

WindowManager *ModeDefault::createWindowManager(Workspace *workspace)
{
    return new WindowManagerDefault(workspace, name());
}
