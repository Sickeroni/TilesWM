#include "mode_simple.h"

#include "window_manager_simple.h"

ModeSimple::ModeSimple() : Mode("floating.simple")
{
}

WindowManager *ModeSimple::createWindowManager(Workspace *workspace)
{
    return new WindowManagerSimple(workspace, this);
}
