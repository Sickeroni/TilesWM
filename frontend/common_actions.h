#ifndef __COMMON_ACTIONS_H__
#define __COMMON_ACTIONS_H__

#include "key_grab_handler.h"
#include "action_set.h"
#include "key_binding_set.h"

class CommonActions final : public KeyGrabHandler
{
public:
    CommonActions();
    ~CommonActions();

    void createKeyBindings();

protected:
    virtual void performAction(int id) override;
    virtual void performComplexAction(const ComplexAction *action, const ComplexAction::Parameters *parameters) override;
    virtual const KeyBindingSet *keyBindings() override;

private:
    enum {
        ACTION_RELOAD_CONFIG,
        ACTION_TOGGLE_MAXIMIZE,
        ACTION_LAYOUT,
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
        ACTION_TOGGLE_DECORATION,
        ACTION_UPDATE_CLIENT_GEOMETRY,
        ACTION_QUIT
    };

//     static void focusPrevClient();
//     static void focusNextClient();
//     static void focusPrevChild(ContainerContainer *container);
//     static void focusNextChild(ContainerContainer *container);
//     static void focusSibling(ContainerUtil::Direction where);

    ActionSet _actions;
    KeyBindingSet _key_bindings;
};

#endif
