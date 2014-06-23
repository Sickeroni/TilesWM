#include "x11_shortcut_set.h"

#include "x11_application.h"
#include "string_util.h"
#include "common.h"

X11ShortcutSet::X11ShortcutSet(std::string name) : ShortcutSet(name)
{
}

X11ShortcutSet::~X11ShortcutSet()
{
    clear();
}

void X11ShortcutSet::clear()
{
    for(std::list<Shortcut>::iterator it = _shortcuts.begin();
            it != _shortcuts.end();
            it++)
    {
        X11Application::self()->releaseKeyGrab(it->key_sequence);
    }
    _shortcuts.clear();
}

void X11ShortcutSet::createShortcut(std::string key_sequence_str, HandlerFunc handler_func)
{
    std::vector<std::string> tokens;
    StringUtil::tokenize(key_sequence_str, '+', tokens);

    X11Global::KeySequence key_sequence(NoSymbol, 0);

    for (size_t i = 0; i < tokens.size(); i++) {
        std::string &s = tokens[i];

        if (s.compare("CTRL") == 0)
            key_sequence.mod_mask |= ControlMask;
        else if (s.compare("MOD1") == 0)
            key_sequence.mod_mask |= Mod1Mask;
        else if (s.compare("SHIFT") == 0)
            key_sequence.mod_mask |= ShiftMask;
        else {
            key_sequence.key_sym = XStringToKeysym(s.c_str());
            if (key_sequence.key_sym == NoSymbol) {
                debug<<"invalid key symbol"<<s<<"in key sequence"<<key_sequence_str;
                return;
            }
            break;
        }
    }

    if (key_sequence.key_sym == NoSymbol)
        debug<<"missing key symbol in key sequence"<<key_sequence_str;
    else if (X11Application::self()->addKeyGrab(key_sequence))
        _shortcuts.push_back(Shortcut(key_sequence, handler_func));
}

bool X11ShortcutSet::handleKeyPress(const X11Global::KeySequence &key_sequence) const
{
    for(std::list<Shortcut>::const_iterator it = _shortcuts.begin();
            it != _shortcuts.end();
            it++)
    {
        if (it->key_sequence.equals(key_sequence)) {
            it->handler_func();
            return true;
        }
    }

    return false;
}
