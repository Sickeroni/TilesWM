#ifndef __KEY_GRAB_HANDLER_H__
#define __KEY_GRAB_HANDLER_H__

#include "action.h"

class KeyBindingSet;
class AbstractKeySequence;

class KeyGrabHandler
{
public:
    bool handleKeySequence(const AbstractKeySequence *sequence);

protected:
    virtual void performAction(int id) = 0;
    virtual void performComplexAction(const ComplexAction *action, const ComplexAction::Parameters *parameters) = 0;
    virtual const KeyBindingSet *keyBindings() = 0;
};

#endif
