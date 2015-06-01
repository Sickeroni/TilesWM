#include "action_set.h"

#include "action.h"

ActionSet::~ActionSet()
{
    for(Action *action : _actions)
        delete action;
    _actions.clear();
}

void ActionSet::createAction(std::string action_name, int id)
{
    assert(find(action_name) == 0);

    _actions.push_back(new Action(action_name, id));
}

void ActionSet::createAction(std::string action_name, ComplexAction *complex_action)
{
    assert(find(action_name) == 0);

    _actions.push_back(new Action(action_name, complex_action));
}

const Action *ActionSet::find(std::string action_name) const
{
    for(const Action *action : _actions) {
        if (action->name == action_name)
            return action;
    }

    return 0;
}
