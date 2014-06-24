#ifndef __WINDOW_MANAGER_DEFAULT_H__
#define __WINDOW_MANAGER_DEFAULT_H__

#include "window_manager.h"
#include "container_util.h"

class ContainerContainer;

class WindowManagerDefault final : public WindowManager
{
public:
    WindowManagerDefault(Workspace *workspace, std::string action_set_name);
    ~WindowManagerDefault();

    // ShortcutSet::Handler implementation
    virtual void handleShortcut(int id) override;

    // ActionSet implementation
    virtual void initShortcuts() override;

    virtual ClientContainer *activeClientContainer() override;
    virtual void manageClient(Client *client) override;
    virtual void layout() override;

private:
    enum {
        ACTION_MOVE_CLIENT_LEFT,
        ACTION_MOVE_CLIENT_RIGHT,
        ACTION_MOVE_CLIENT_UP,
        ACTION_MOVE_CLIENT_DOWN,
        ACTION_DELETE_EMPTY_CONTAINERS,
        ACTION_TOGGLE_EXPANDING,
        ACTION_TOGGLE_PARENT_EXPANDING,
        ACTION_INC_WIDTH,
        ACTION_DEC_WIDTH,
        ACTION_INC_HEIGHT,
        ACTION_DEC_HEIGHT,
        ACTION_SET_FIXED_SIZE_TO_MINIMUM
    };

    void moveClient(ContainerUtil::Direction direction);
    void changeSize(bool horizontal, int delta);
    void setFixedSizeToMinimum();

    ContainerContainer *_root_container = 0;
};

#endif
