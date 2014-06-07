#include "action_set.h"

#include "shortcut_set.h"
#include "application.h"

ActionSet::ActionSet(std::string name) :
    _shortcuts(Application::self()->createShortcutSet(name))
{
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
