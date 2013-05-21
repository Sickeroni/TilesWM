#include "x11_shortcut.h"

#include "x11_application.h"
#include "x11_global.h"
#include "common.h"

#include <stdlib.h>


using namespace X11Global;


X11Shortcut::KeySymMapping X11Shortcut::_key_sym_index;


X11Shortcut::X11Shortcut(const char *key_sym, ModMask mod_mask, HandlerFunc handler_func) :
    _key_sym(XStringToKeysym(key_sym)), _mod_mask(mod_mask), _handler_func(handler_func)
{
    ShortcutList &list = _key_sym_index[_key_sym];

    for (unsigned int i = 0; i < list.size(); i++) {
        X11Shortcut *shortcut = list[i];
        if (shortcut->_mod_mask == _mod_mask) {
            abort();
        }
    }

    list.push_back(this);

    XGrabKey(dpy(), XKeysymToKeycode(dpy(), _key_sym),
             _mod_mask, X11Application::root(), true, GrabModeAsync, GrabModeAsync);
}

X11Shortcut::~X11Shortcut()
{
    XUngrabKey(dpy(), XKeysymToKeycode(dpy(), _key_sym),
               _mod_mask, X11Application::root());

    ShortcutList &list = _key_sym_index[_key_sym];

    ShortcutList::iterator it = list.begin();

    while (it != list.end()) {
        if (this == *it) {
            list.erase(it);
            break;
        }
        it++;
    }
}

bool X11Shortcut::handleKeyPress(const XKeyEvent &ev)
{
    debug;

    KeySym key_sym = XLookupKeysym(const_cast<XKeyEvent*>(&ev), 0);
    if (key_sym != NoSymbol) {
        KeySymMapping::iterator it = _key_sym_index.find(key_sym);
        if (it != _key_sym_index.end()) {
            ShortcutList &list = it->second;
            for (unsigned int i = 0; i < list.size(); i++) {
                X11Shortcut *shortcut = list[i];
                if (shortcut->_mod_mask == ev.state) {
                    shortcut->_handler_func();
                    return true;
                }
            }
        }
    }
    return false;
}
