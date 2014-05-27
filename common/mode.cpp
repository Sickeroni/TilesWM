#include "mode.h"

#include "application.h"
#include "shortcut_set.h"

Mode::Mode() :
    _shortcuts(Application::self()->createShortcutSet())
{
}

Mode::~Mode()
{
    delete _shortcuts;
    _shortcuts = 0;
}
