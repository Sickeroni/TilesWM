#ifndef __KEY_BINDING_SET__
#define __KEY_BINDING_SET__

#include "action.h"

#include <vector>

class KeyGrabSet;
class ActionSet;
struct AbstractKeySequence;

class KeyBindingSet
{
public:
    struct Binding
    {
        AbstractKeySequence *key_sequence = 0;
        const Action *action = 0;
        ComplexAction::Parameters *parameters = 0;
    };

    KeyBindingSet(const std::string &name, const ActionSet *actions);
    ~KeyBindingSet();

    void clear();
    void createBindings();
    const Binding *find(const AbstractKeySequence *sequence) const;

private:
    std::string _config_group;
    const ActionSet *_actions = 0;
    std::vector<Binding> _bindings;
};

#endif
