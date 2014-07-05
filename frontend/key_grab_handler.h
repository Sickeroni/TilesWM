#ifndef __KEY_GRAB_HANDLER_H__
#define __KEY_GRAB_HANDLER_H__

#include "key_grab_handler_base.h"
#include "action.h"

class KeyBindingSet;

class KeyGrabHandler : public KeyGrabHandlerBase
{
public:
    virtual void handleKeyGrabPressed(int grab_id) override;

protected:
    virtual void performAction(int id) = 0;
    virtual void performComplexAction(ComplexAction *action, ComplexAction::Parameters *parameters) = 0;
    virtual const KeyBindingSet *keyBindings() = 0;
};

#endif
