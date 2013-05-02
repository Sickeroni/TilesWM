#include "x11_key_handler.h"

X11KeyHandler::~X11KeyHandler()
{
    for(unsigned int i = 0; i < _shortcuts.size(); i++)
        delete _shortcuts[i];
    _shortcuts.clear();
}

void X11KeyHandler::createShortcut(const char *key_sym, X11Shortcut::ModMask mod_mask,
                                   X11Shortcut::HandlerFunc handler_func)
{
    _shortcuts.push_back(new X11Shortcut(key_sym, mod_mask, handler_func));
}
