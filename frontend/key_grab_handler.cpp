#include "key_grab_handler.h"
#include "key_binding_set.h"

void KeyGrabHandler::handleKeyGrabPressed(int grab_id)
{
    const KeyBindingSet::Binding *binding = keyBindings()->find(grab_id);
    if (binding) {
        switch (binding->action->type) {
            case Action::TYPE_SIMPLE:
                performAction(binding->action->id);
                break;
            case Action::TYPE_COMPLEX:
                performComplexAction(binding->action->complex_action, binding->parameters);
                break;
        }
    }
}