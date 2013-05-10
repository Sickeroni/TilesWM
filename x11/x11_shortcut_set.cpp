#include "x11_shortcut_set.h"

X11ShortcutSet::~X11ShortcutSet()
{
    for(unsigned int i = 0; i < _shortcuts.size(); i++)
        delete _shortcuts[i];
    _shortcuts.clear();
}

void X11ShortcutSet::createShortcut(const char *key_sym, X11Shortcut::ModMask mod_mask,
                                   X11Shortcut::HandlerFunc handler_func)
{
    _shortcuts.push_back(new X11Shortcut(key_sym, mod_mask, handler_func));
}
