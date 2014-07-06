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
        int key_grab_id = 0;
        const Action *action = 0;
        ComplexAction::Parameters *parameters = 0;
    };

    KeyBindingSet(const char *config_group, const ActionSet *actions);
    ~KeyBindingSet();

    void clear();
    void createBindings();
    const Binding *find(int key_grab_id) const;
    const KeyGrabSet *keyGrabs() { return _key_grabs; }

private:
    std::string _config_group;
    const ActionSet *_actions = 0;
    std::vector<Binding> _bindings;
    KeyGrabSet *_key_grabs = 0;
};

#endif
