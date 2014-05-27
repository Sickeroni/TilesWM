#include "mode.h"

#include "application.h"
#include "shortcut_set.h"

Mode::Mode(std::string name) :
    _shortcuts(Application::self()->createShortcutSet(name))
{
}

Mode::~Mode()
{
    delete _shortcuts;
    _shortcuts = 0;
}
