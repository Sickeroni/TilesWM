#include "mode_mixed.h"
#include "window_manager_mixed.h"

#include <sstream>

ModeMixed::ModeMixed(Mode *floating_mode, Mode *tiling_mode) : Mode("mixed"),
    _floating_mode(floating_mode),
    _tiling_mode(tiling_mode)
{
    std::stringstream display_name;
    display_name<<"mixed ("<<_tiling_mode->displayName()<<" + "<<_floating_mode->displayName()<<")";
    setDisplayName(display_name.str());
}

ModeMixed::~ModeMixed()
{
    delete _floating_mode;
    _floating_mode = 0;
    delete _tiling_mode;
    _tiling_mode = 0;
}

WindowManager *ModeMixed::createWindowManager(Workspace *workspace)
{
    return new WindowManagerMixed(workspace, this, _tiling_mode, _floating_mode);
}

void ModeMixed::createKeyBindings()
{
    Mode::createKeyBindings();
    _tiling_mode->createKeyBindings();
    _floating_mode->createKeyBindings();
}

void ModeMixed::clearKeyBindings()
{
    _tiling_mode->clearKeyBindings();
    _floating_mode->clearKeyBindings();
    Mode::clearKeyBindings();
}
