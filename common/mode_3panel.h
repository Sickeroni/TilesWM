#ifndef __MODE_3PANEL_H__
#define __MODE_3PANEL_H__

#include "mode.h"
#include "container_util.h"

class ClientContainer;

class Mode3Panel final : public Mode
{
public:
    Mode3Panel();

    virtual void activate(Workspace *workspace) override;
    virtual void initShortcuts() override;
    virtual void handleShortcut(int id);
    virtual void tileClient(Client *client, ContainerContainer *root_container) override;

private:
    enum {
        ACTION_MOVE_CLIENT_LEFT,
        ACTION_MOVE_CLIENT_RIGHT,
        ACTION_MOVE_CLIENT_UP,
        ACTION_MOVE_CLIENT_DOWN,
        ACTION_MOVE_HSPLIT_LEFT,
        ACTION_MOVE_HSPLIT_RIGHT,
        ACTION_MOVE_HSPLIT_UP,
        ACTION_MOVE_HSPLIT_DOWN,
        ACTION_TOGGLE_PRIMARY_EXPANDING,
        ACTION_TOGGLE_SECONDARY_EXPANDING,
        ACTION_SET_PRIMARY_SLAVE_TO_MINIMUM,
        ACTION_SET_SECONDARY_SLAVE_TO_MINIMUM
    };

    static void moveClient(ContainerUtil::Direction direction);
    static void moveSplitter(bool horizontal, int delta);

    static void togglePrimaryExpanding();
    static void toggleSecondaryExpanding();

    static void setPrimarySlaveToMinimum();
    static void setSecondarySlaveToMinimum();
};

#endif
