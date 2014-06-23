#ifndef __MODE_DEFAULT_H__
#define __MODE_DEFAULT_H__

#include "mode.h"
#include "container_util.h"

class ModeDefault final : public Mode
{
public:
    ModeDefault();

    virtual void activate(Workspace *workspace) override;
    virtual void initShortcuts() override;
    virtual void handleShortcut(int id) override;
    virtual void tileClient(Client *client, ContainerContainer *root_container) override;

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

    static void moveClient(ContainerUtil::Direction direction);
    static void changeSize(bool horizontal, int delta);
    static void setFixedSizeToMinimum();
};


#endif
