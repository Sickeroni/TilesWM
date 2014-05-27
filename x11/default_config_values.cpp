#include "default_config_values.h"

#include "config.h"

void setDefaultConfigValues()
{
    Config::setValue("shortcuts.main.layout", "MOD1+q");
    Config::setValue("shortcuts.main.layout", "MOD1+l");
    Config::setValue("shortcuts.main.rotate", "MOD1+r");
    Config::setValue("shortcuts.main.focusLeft", "MOD1+KP_Left");
    Config::setValue("shortcuts.main.focusRight", "MOD1+KP_Right");
    Config::setValue("shortcuts.main.focusUp", "MOD1+KP_Up");
    Config::setValue("shortcuts.main.focusDown", "MOD1+KP_Down");
    Config::setValue("shortcuts.main.runTerminal", "MOD1+t");
    Config::setValue("shortcuts.main.redraw", "MOD1+d");
    Config::setValue("shortcuts.main.focusPrevClient", "MOD1+comma");
    Config::setValue("shortcuts.main.focusNextClient", "MOD1+period");
    Config::setValue("shortcuts.main.runProgram", "MOD1+F2");
    Config::setValue("shortcuts.main.changeMode", "CTRL+m");
    Config::setValue("shortcuts.main.quit", "MOD1+q");

    Config::setValue("shortcuts.default.moveClientLeft", "SHIFT+MOD1+KP_Left");
    Config::setValue("shortcuts.default.moveClientRight", "SHIFT+MOD1+KP_Right");
    Config::setValue("shortcuts.default.moveClientUp", "SHIFT+MOD1+KP_Up");
    Config::setValue("shortcuts.default.moveClientDown", "SHIFT+MOD1+KP_Down");
    Config::setValue("shortcuts.default.deleteEmptyContainers", "MOD1+c");
    Config::setValue("shortcuts.default.toggleExpanding", "MOD1+e");
    Config::setValue("shortcuts.default.toggleParentExpanding", "CTRL+MOD1+e");
    Config::setValue("shortcuts.default.incWidth", "CTRL+MOD1+KP_Right");
    Config::setValue("shortcuts.default.decWidth", "CTRL+MOD1+KP_Left");
    Config::setValue("shortcuts.default.incHeight", "CTRL+MOD1+KP_Down");
    Config::setValue("shortcuts.default.decHeight", "CTRL+MOD1+KP_Up");
    Config::setValue("shortcuts.default.toggleExpanding", "MOD1+e");
    Config::setValue("shortcuts.default.toggleParentExpanding", "CTRL+MOD1+e");

    Config::setValue("shortcuts.3panel.moveClientLeft", "SHIFT+MOD1+KP_Left");
    Config::setValue("shortcuts.3panel.moveClientRight", "SHIFT+MOD1+KP_Right");
    Config::setValue("shortcuts.3panel.moveClientUp", "SHIFT+MOD1+KP_Up");
    Config::setValue("shortcuts.3panel.moveClientDown", "SHIFT+MOD1+KP_Down");
    Config::setValue("shortcuts.3panel.moveHSplitRight", "CTRL+MOD1+KP_Right");
    Config::setValue("shortcuts.3panel.moveHSplitLeft", "CTRL+MOD1+KP_Left");
    Config::setValue("shortcuts.3panel.moveVSplitDown", "CTRL+MOD1+KP_Down");
    Config::setValue("shortcuts.3panel.moveVSplitUp", "CTRL+MOD1+KP_Up");
    Config::setValue("shortcuts.3panel.togglePrimaryExpanding", "MOD1+e");
    Config::setValue("shortcuts.3panel.toggleSecondaryExpanding", "CTRL+MOD1+e");
}
