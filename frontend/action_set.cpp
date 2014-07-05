#include "action_set.h"

// #include "shortcut_set.h"
#include "application.h"

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
    UNIMPLEMENTED
}

void ActionSet::createAction(std::string action_name, int id)
{
    UNIMPLEMENTED
}

const Action *ActionSet::find(std::string action_name) const
{
    UNIMPLEMENTED
    return 0;
}
