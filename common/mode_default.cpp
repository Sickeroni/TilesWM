#include "mode_default.h"

#include "actions.h"
#include "shortcut_set.h"

#include <X11/Xlib.h> //HACK

void ModeDefault::activate(Workspace *workspace)
{
}

void ModeDefault::initShortcuts()
{
    shortcuts()->createShortcut("KP_Left", Mod1Mask | ShiftMask, &Actions::moveClientLeft);
    shortcuts()->createShortcut("KP_Right", Mod1Mask | ShiftMask, &Actions::moveClientRight);
    shortcuts()->createShortcut("KP_Up", Mod1Mask | ShiftMask, &Actions::moveClientUp);
    shortcuts()->createShortcut("KP_Down", Mod1Mask | ShiftMask, &Actions::moveClientDown);
    shortcuts()->createShortcut("c", Mod1Mask, &Actions::deleteEmptyContainers);

    shortcuts()->createShortcut("e", Mod1Mask, &Actions::toggleExpanding);
    shortcuts()->createShortcut("e", ControlMask | Mod1Mask, &Actions::toggleParentExpanding);

    shortcuts()->createShortcut("KP_Right", ControlMask | Mod1Mask, &Actions::incWidth);
    shortcuts()->createShortcut("KP_Left", ControlMask | Mod1Mask, &Actions::decWidth);
    shortcuts()->createShortcut("KP_Down", ControlMask | Mod1Mask, &Actions::incHeight);
    shortcuts()->createShortcut("KP_Up", ControlMask | Mod1Mask, &Actions::decHeight);
}
