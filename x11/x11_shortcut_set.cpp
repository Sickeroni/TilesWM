#include "x11_shortcut_set.h"

#include "string_util.h"
#include "common.h"

X11ShortcutSet::X11ShortcutSet(std::string name) : ShortcutSet(name)
{
}

X11ShortcutSet::~X11ShortcutSet()
{
    for(X11Shortcut::List::iterator it = _shortcuts.begin();
                it != _shortcuts.end();
                it++)
        delete *it;
    _shortcuts.clear();
}

void X11ShortcutSet::createShortcut(std::string key_sequence, HandlerFunc handler_func)
{
    std::vector<std::string> tokens;
    StringUtil::tokenize(key_sequence, '+', tokens);

    X11Shortcut::ModMask mod_mask = 0;
    KeySym key_sym = NoSymbol;

    for (size_t i = 0; i < tokens.size(); i++) {
        std::string &s = tokens[i];

        if (s.compare("CTRL") == 0)
            mod_mask |= ControlMask;
        else if (s.compare("MOD1") == 0)
            mod_mask |= Mod1Mask;
        else if (s.compare("SHIFT") == 0)
            mod_mask |= ShiftMask;
        else {
            key_sym = XStringToKeysym(s.c_str());
            if (key_sym == NoSymbol) {
                debug<<"invalid key symbol"<<s<<"in key sequence"<<key_sequence;
                return;
            }
            break;
        }
    }

    if (key_sym == NoSymbol)
        debug<<"missing key symbol in key sequence"<<key_sequence;
    else
        _shortcuts.push_back(new X11Shortcut(key_sym, mod_mask, handler_func));
}
