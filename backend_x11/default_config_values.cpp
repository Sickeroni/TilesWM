#include "config.h"

void Config::setDefaults()
{
    clear();

    Section *s = createSection("keybindings.common");
    s->addEntry("MOD1+q", "quit");
    s->addEntry("SHIFT+MOD1+r", "reloadConfig");
    s->addEntry("MOD1+m", "toggleMaximize");
    s->addEntry("MOD1+l", "layout");
    s->addEntry("MOD1+r", "rotate");
    s->addEntry("MOD1+KP_Left", "focusLeft");
    s->addEntry("MOD1+KP_Right", "focusRight");
    s->addEntry("MOD1+KP_Up", "focusUp");
    s->addEntry("MOD1+KP_Down", "focusDown");
//     s->addEntry("MOD1+t", "runTerminal");
    s->addEntry("MOD1+d", "redraw");
    s->addEntry("MOD1+comma", "focusPrevClient");
    s->addEntry("MOD1+period", "focusNextClient");
    s->addEntry("MOD1+F2", "runProgram");
    s->addEntry("CTRL+m", "changeMode");
    s->addEntry("MOD1+f", "focusActiveClient");
    s->addEntry("MOD1+x", "closeActiveClient");
    s->addEntry("MOD1+t", "run lxterminal");

    s = createSection("keybindings.tiling.common");
    //FIXME

    s = createSection("keybindings.tiling.default");
    s->addEntry("SHIFT+MOD1+KP_Left", "moveClientLeft");
    s->addEntry("SHIFT+MOD1+KP_Right", "moveClientRight");
    s->addEntry("SHIFT+MOD1+KP_Up", "moveClientUp");
    s->addEntry("SHIFT+MOD1+KP_Down", "moveClientDown");
    s->addEntry("MOD1+c", "deleteEmptyContainers");
    s->addEntry("CTRL+MOD1+KP_Right", "incWidth");
    s->addEntry("CTRL+MOD1+KP_Left", "decWidth");
    s->addEntry("CTRL+MOD1+KP_Down", "incHeight");
    s->addEntry("CTRL+MOD1+KP_Up", "decHeight");
    s->addEntry("MOD1+e", "toggleExpanding");
    s->addEntry("CTRL+MOD1+e", "toggleParentExpanding");
    s->addEntry("SHIFT+MOD1+m", "setFixedSizeToMinimum");

    s = createSection("keybindings.tiling.3panel");
    s->addEntry("SHIFT+MOD1+KP_Left", "moveClientLeft");
    s->addEntry("SHIFT+MOD1+KP_Right", "moveClientRight");
    s->addEntry("SHIFT+MOD1+KP_Up", "moveClientUp");
    s->addEntry("SHIFT+MOD1+KP_Down", "moveClientDown");
    s->addEntry("CTRL+MOD1+KP_Right", "moveHSplitRight");
    s->addEntry("CTRL+MOD1+KP_Left", "moveHSplitLeft");
    s->addEntry("CTRL+MOD1+KP_Down", "moveVSplitDown");
    s->addEntry("CTRL+MOD1+KP_Up", "moveVSplitUp");
    s->addEntry("MOD1+e", "togglePrimaryExpanding");
    s->addEntry("CTRL+MOD1+e", "toggleSecondaryExpanding");
    s->addEntry("CTRL+MOD1+m", "setPrimarySlaveToMinimum");
    s->addEntry("SHIFT+MOD1+m", "setSecondarySlaveToMinimum");
}
