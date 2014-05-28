#include "config.h"

const Config::Map Config::_default_values =
{
    { "shortcuts.main.toggleMaximize", "MOD1+m" },
    { "shortcuts.main.layout", "MOD1+l" },
    { "shortcuts.main.rotate", "MOD1+r" },
    { "shortcuts.main.focusLeft", "MOD1+KP_Left" },
    { "shortcuts.main.focusRight", "MOD1+KP_Right" },
    { "shortcuts.main.focusUp", "MOD1+KP_Up" },
    { "shortcuts.main.focusDown", "MOD1+KP_Down" },
    { "shortcuts.main.runTerminal", "MOD1+t" },
    { "shortcuts.main.redraw", "MOD1+d" },
    { "shortcuts.main.focusPrevClient", "MOD1+comma" },
    { "shortcuts.main.focusNextClient", "MOD1+period" },
    { "shortcuts.main.runProgram", "MOD1+F2" },
    { "shortcuts.main.changeMode", "CTRL+m" },
    { "shortcuts.main.quit", "MOD1+q" },

    { "shortcuts.default.moveClientLeft", "SHIFT+MOD1+KP_Left" },
    { "shortcuts.default.moveClientRight", "SHIFT+MOD1+KP_Right" },
    { "shortcuts.default.moveClientUp", "SHIFT+MOD1+KP_Up" },
    { "shortcuts.default.moveClientDown", "SHIFT+MOD1+KP_Down" },
    { "shortcuts.default.deleteEmptyContainers", "MOD1+c" },
    { "shortcuts.default.toggleExpanding", "MOD1+e" },
    { "shortcuts.default.toggleParentExpanding", "CTRL+MOD1+e" },
    { "shortcuts.default.incWidth", "CTRL+MOD1+KP_Right" },
    { "shortcuts.default.decWidth", "CTRL+MOD1+KP_Left" },
    { "shortcuts.default.incHeight", "CTRL+MOD1+KP_Down" },
    { "shortcuts.default.decHeight", "CTRL+MOD1+KP_Up" },
    { "shortcuts.default.toggleExpanding", "MOD1+e" },
    { "shortcuts.default.toggleParentExpanding", "CTRL+MOD1+e" },

    { "shortcuts.3panel.moveClientLeft", "SHIFT+MOD1+KP_Left" },
    { "shortcuts.3panel.moveClientRight", "SHIFT+MOD1+KP_Right" },
    { "shortcuts.3panel.moveClientUp", "SHIFT+MOD1+KP_Up" },
    { "shortcuts.3panel.moveClientDown", "SHIFT+MOD1+KP_Down" },
    { "shortcuts.3panel.moveHSplitRight", "CTRL+MOD1+KP_Right" },
    { "shortcuts.3panel.moveHSplitLeft", "CTRL+MOD1+KP_Left" },
    { "shortcuts.3panel.moveVSplitDown", "CTRL+MOD1+KP_Down" },
    { "shortcuts.3panel.moveVSplitUp", "CTRL+MOD1+KP_Up" },
    { "shortcuts.3panel.togglePrimaryExpanding", "MOD1+e" },
    { "shortcuts.3panel.toggleSecondaryExpanding", "CTRL+MOD1+e" },
};