#include "x11_shortcut_set.h"

X11ShortcutSet::~X11ShortcutSet()
{
    for(X11Shortcut::List::iterator it = _shortcuts.begin();
                it != _shortcuts.end();
                it++)
        delete *it;
    _shortcuts.clear();
}

void X11ShortcutSet::createShortcut(const char *key_sym, X11Shortcut::ModMask mod_mask,
                                   ShortcutSet::HandlerFunc handler_func)
{
    _shortcuts.push_back(new X11Shortcut(key_sym, mod_mask, handler_func));
}
