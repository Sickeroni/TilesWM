#include "x11_default_key_bindings.h"

#include "actions.h"

X11DefaultKeyBindings::X11DefaultKeyBindings()
{
    createShortcut("l", Mod1Mask, &Actions::layout);
    createShortcut("r", Mod1Mask, &Actions::rotate);
    createShortcut("KP_Left", Mod1Mask, &Actions::focusLeft);
    createShortcut("KP_Right", Mod1Mask, &Actions::focusRight);
    createShortcut("KP_Up", Mod1Mask, &Actions::focusUp);
    createShortcut("KP_Down", Mod1Mask, &Actions::focusDown);
    createShortcut("t", Mod1Mask, &Actions::runTerminal);
    createShortcut("d", Mod1Mask, &Actions::redraw);
    createShortcut("comma", Mod1Mask, &Actions::focusPrevClient);
    createShortcut("period", Mod1Mask, &Actions::focusNextClient);
    createShortcut("KP_Left", Mod1Mask | ShiftMask, &Actions::moveClientLeft);
    createShortcut("KP_Right", Mod1Mask | ShiftMask, &Actions::moveClientRight);
    createShortcut("KP_Up", Mod1Mask | ShiftMask, &Actions::moveClientUp);
    createShortcut("KP_Down", Mod1Mask | ShiftMask, &Actions::moveClientDown);
    createShortcut("c", Mod1Mask, &Actions::deleteEmptyContainers);
    createShortcut("F2", Mod1Mask, &Actions::runProgram);

    createShortcut("m", Mod1Mask, &Actions::toggleMaximize);
    createShortcut("e", Mod1Mask, &Actions::toggleExpanding);
    createShortcut("e", ControlMask | Mod1Mask, &Actions::toggleParentExpanding);

    createShortcut("KP_Right", ControlMask | Mod1Mask, &Actions::incWidth);
    createShortcut("KP_Left", ControlMask | Mod1Mask, &Actions::decWidth);
    createShortcut("KP_Down", ControlMask | Mod1Mask, &Actions::incHeight);
    createShortcut("KP_Up", ControlMask | Mod1Mask, &Actions::decHeight);
}
