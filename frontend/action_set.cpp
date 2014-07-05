#include "action_set.h"

// #include "shortcut_set.h"
#include "action.h"

#if 0
ActionSet::ActionSet(std::string name) :
    _shortcuts(Application::self()->createShortcutSet(name))
{
    _shortcuts->setHandler(this);
}

ActionSet::~ActionSet()
{
    delete _shortcuts;
    _shortcuts = 0;
}

void ActionSet::reloadShortcuts()
{
    _shortcuts->clear();
    initShortcuts();
}
#endif

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

const Action *ActionSet::find(std::string action_name) const
{
    for(const Action *action : _actions) {
        if (action->name == action_name)
            return action;
    }

    return 0;
}
