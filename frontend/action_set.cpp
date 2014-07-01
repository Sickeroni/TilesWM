#include "action_set.h"

#include "shortcut_set.h"
#include "application.h"

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
