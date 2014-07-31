#ifndef __KEY_BINDING_SET__
#define __KEY_BINDING_SET__

#include "action.h"

#include <vector>

class KeyGrabSet;
class ActionSet;

class KeyBindingSet
{
public:
    struct Binding
    {
        size_t key_grab_id = 0;
        const Action *action = 0;
        ComplexAction::Parameters *parameters = 0;
    };

    KeyBindingSet(const std::string &name, const ActionSet *actions);
    ~KeyBindingSet();

    void clear();
    void createBindings();
    const Binding *find(size_t key_grab_id) const;
    const KeyGrabSet *keyGrabs() const { return _key_grabs; }

private:
    std::string _config_group;
    const ActionSet *_actions = 0;
    std::vector<Binding> _bindings;
    KeyGrabSet *_key_grabs = 0;
};

#endif
