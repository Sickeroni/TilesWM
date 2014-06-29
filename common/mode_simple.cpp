#include "mode_simple.h"

#include "window_manager_simple.h"

ModeSimple::ModeSimple() : Mode("simpe")
{
}

WindowManager *ModeSimple::createWindowManager(Workspace *workspace)
{
    return new WindowManagerSimple(workspace, name());
}
