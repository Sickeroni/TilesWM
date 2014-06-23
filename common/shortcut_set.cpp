#include "shortcut_set.h"

#include "config.h"
#include "common.h"

void ShortcutSet::createAction(std::string action_name, int id)
{
    std::string config_key = std::string("shortcuts.") + _name + '.' + action_name;
    std::string key_sequence = Config::value(config_key);
    if (!key_sequence.empty())
        createShortcut(key_sequence, id);
    else
        debug<<"no shortcut defined for action"<<action_name;
}
