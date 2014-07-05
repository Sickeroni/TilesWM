#ifndef __KEY_BINDING_SET__
#define __KEY_BINDING_SET__

#include "action.h"

class KeyGrabSet;
class ActionSet;

class KeyBindingSet
{
public:
    struct Binding
    {
        int key_grab_id = 0;
        Action *action = 0;
        ComplexAction::Parameters *parameters = 0;
    };

    KeyBindingSet(const std::string &config_group, const ActionSet *actions);

    void clear();
    void createBindings();
    const Binding *find(int key_grab_id) const;

private:
    std::string _config_group;
    KeyGrabSet *_key_grabs = 0;
    const ActionSet *_actions = 0;
};

#endif
