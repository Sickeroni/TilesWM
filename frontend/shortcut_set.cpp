#include "shortcut_set.h"

#include "key_sequence.h"
#include "config.h"
#include "common.h"


Shortcut::~ShortcutSet()
{
    clear();
}

void ShortcutSet::clear()
{
    for(std::list<Shortcut>::iterator it = _shortcuts.begin();
            it != _shortcuts.end();
            it++)
    {
        delete it->key_sequence;
        it->key_sequence = 0;
    }
    _shortcuts.clear();
}

void ShortcutSet::createAction(std::string action_name, int id)
{
    std::string config_key = std::string("shortcuts.") + _name + '.' + action_name;
    std::string key_sequence_str = Config::value(config_key);

    if (!key_sequence_str.empty()) {
        KeySequence *key_sequence = Application::backend()->grabKeySequence(key_sequence_str);
        if (key_sequence)
            _shortcuts.push_back(Shortcut(key_sequence, id));
    } else
        debug<<"no shortcut defined for action"<<action_name;
}

bool ShortcutSet::handleKeyPress(const KeySequence *key_sequence) const
{
    debug;
    for(std::list<Shortcut>::const_iterator it = _shortcuts.begin();
            it != _shortcuts.end();
            it++)
    {
        if (it->key_sequence->equals(key_sequence)) {
            handler()->handleShortcut(it->id);
            return true;
        }
    }

    return false;
}
