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
    virtual void tileClient(Client *client, ContainerContainer *root_container) override;

private:
    static void moveClient(ContainerUtil::Direction direction);
    static void moveClientLeft();
    static void moveClientRight();
    static void moveClientUp();
    static void moveClientDown();
    static void deleteEmptyContainers();
    static void toggleExpanding();
    static void toggleParentExpanding();
    static void changeSize(bool horizontal, int delta);
    static void incWidth();
    static void decWidth();
    static void incHeight();
    static void decHeight();
    static void setFixedSizeToMinimum();
};


#endif
