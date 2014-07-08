#include "mode_default.h"

#include "window_manager_default.h"

ModeDefault::ModeDefault() : Mode("tiling.default")
{
    _actions.createAction("moveClientLeft", WindowManagerDefault::ACTION_MOVE_CLIENT_LEFT);
    _actions.createAction("moveClientRight", WindowManagerDefault::ACTION_MOVE_CLIENT_RIGHT);
    _actions.createAction("moveClientUp", WindowManagerDefault::ACTION_MOVE_CLIENT_UP);
    _actions.createAction("moveClientDown", WindowManagerDefault::ACTION_MOVE_CLIENT_DOWN);
    _actions.createAction("deleteEmptyContainers", WindowManagerDefault::ACTION_DELETE_EMPTY_CONTAINERS);

    _actions.createAction("toggleExpanding", WindowManagerDefault::ACTION_TOGGLE_EXPANDING);
    _actions.createAction("toggleParentExpanding", WindowManagerDefault::ACTION_TOGGLE_PARENT_EXPANDING);

    _actions.createAction("incWidth", WindowManagerDefault::ACTION_INC_WIDTH);
    _actions.createAction("decWidth", WindowManagerDefault::ACTION_DEC_WIDTH);
    _actions.createAction("incHeight", WindowManagerDefault::ACTION_INC_HEIGHT);
    _actions.createAction("decHeight", WindowManagerDefault::ACTION_DEC_HEIGHT);

    _actions.createAction("setFixedSizeToMinimum", WindowManagerDefault::ACTION_SET_FIXED_SIZE_TO_MINIMUM);

    _actions.createAction("rotate", WindowManagerDefault::ACTION_ROTATE);
}

WindowManager *ModeDefault::createWindowManager(Workspace *workspace)
{
    return new WindowManagerDefault(workspace, this);
}
