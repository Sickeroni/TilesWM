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
    virtual void handleShortcut(int id) override;

private:
    enum {
        ACTION_RELOAD_CONFIG,
        ACTION_TOGGLE_MAXIMIZE,
        ACTION_LAYOUT,
        ACTION_ROTATE,
        ACTION_FOCUS_LEFT,
        ACTION_FOCUS_RIGHT,
        ACTION_FOCUS_UP,
        ACTION_FOCUS_DOWN,
        ACTION_REDRAW,
        ACTION_FOCUS_NEXT_CLIENT,
        ACTION_FOCUS_PREV_CLIENT,
        ACTION_RUN_PROGRAM,
        ACTION_RUN_TERMINAL,
        ACTION_CHANGE_MODE,
        ACTION_CLOSE_ACTIVE_CLIENT,
        ACTION_FOCUS_ACTIVE_CLIENT,
        ACTION_QUIT
    };

//     static void focusPrevClient();
//     static void focusNextClient();
//     static void focusPrevChild(ContainerContainer *container);
//     static void focusNextChild(ContainerContainer *container);
//     static void focusSibling(ContainerUtil::Direction where);
};

#endif
