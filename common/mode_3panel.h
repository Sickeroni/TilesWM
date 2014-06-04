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
    virtual void tileClient(Client *client, ContainerContainer *root_container) override;

private:
    static void moveClient(ContainerUtil::Direction direction);
    static void moveSplitter(bool horizontal, int delta);

    static void moveHSplitRight();
    static void moveHSplitLeft();
    static void moveVSplitDown();
    static void moveVSplitUp();

    static void moveClientLeft();
    static void moveClientRight();
    static void moveClientUp();
    static void moveClientDown();

    static void togglePrimaryExpanding();
    static void toggleSecondaryExpanding();

    static void setPrimarySlaveToMinimum();
    static void setSecondarySlaveToMinimum();
};

#endif
