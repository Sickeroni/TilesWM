#ifndef __MODE_3PANEL_H__
#define __MODE_3PANEL_H__

#include "mode.h"
#include "container.h"
#include "container_util.h"

class ClientContainer;

class Mode3Panel final : public Mode
{
public:
    virtual void activate(Workspace *workspace) override;
    virtual void initShortcuts() override;

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
};

#endif
