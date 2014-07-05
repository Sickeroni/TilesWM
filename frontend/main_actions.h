#ifndef __MAIN_ACTIONS_H__
#define __MAIN_ACTIONS_H__

#include "key_grab_handler.h"
#include "action_set.h"

class MainActions final : public KeyGrabHandler
{
public:
    MainActions();
    ~MainActions();

    virtual const KeyGrabSet *grabs() override { return _key_grabs; }

protected:
    virtual void performAction(int id) override;
    virtual void performComplexAction(ComplexAction *action, ComplexAction::Parameters *parameters) override;
    virtual const KeyBindingSet *keyBindings() override;

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

    ActionSet _actions;
    KeyGrabSet *_key_grabs = 0;
};

#endif
