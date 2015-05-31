#include "key_grab_handler.h"
#include "key_binding_set.h"
#include "common.h"

bool KeyGrabHandler::handleKeySequence(const AbstractKeySequence *sequence) {
    const KeyBindingSet::Binding *binding = keyBindings()->find(sequence);
    if (!binding)
        return false;

    switch (binding->action->type) {
        case Action::TYPE_SIMPLE:
            performAction(binding->action->id);
            break;
        case Action::TYPE_COMPLEX:
            performComplexAction(binding->action->complex_action, binding->parameters);
            break;
    }

    return true;
}
