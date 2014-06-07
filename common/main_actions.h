#ifndef __MAIN_ACTIONS_H__
#define __MAIN_ACTIONS_H__

#include "action_set.h"
#include "container_util.h"

class ContainerContainer;

class MainActions : public ActionSet
{
public:
    MainActions() : ActionSet("main") {}

    virtual void initShortcuts() override;

private:
    static void runTerminal();
    static void runProgram();

    static void toggleMaximize();

    static void focusPrevClient();
    static void focusNextClient();

    static void focusLeft();
    static void focusRight();
    static void focusUp();
    static void focusDown();

    static void redraw();
    static void layout();
    static void rotate();
    static void changeMode();

    static void closeActiveClient();

    static void quit();

    static void focusPrevChild(ContainerContainer *container);
    static void focusNextChild(ContainerContainer *container);
    static void focusSibling(ContainerUtil::Direction where);
};

#endif
