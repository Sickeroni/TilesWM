#include "mode_simple.h"

#include "window_manager_simple.h"

ModeSimple::ModeSimple() : Mode("floating.simple")
{
}

WindowManager *ModeSimple::createWindowManager(Widget *parent_widget)
{
    return new WindowManagerSimple(parent_widget, this);
}
