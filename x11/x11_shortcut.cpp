#include "x11_shortcut.h"

#include "x11_application.h"
#include "x11_global.h"
#include "x11_shortcut_set.h"
#include "workspace.h"
#include "mode.h"
#include "common.h"

#include <stdlib.h>


using namespace X11Global;


X11Shortcut::KeyGrabList X11Shortcut::_key_grabs;

struct X11Shortcut::KeyGrab
{
    KeyGrab(KeySym key_sym, X11Shortcut::ModMask mod_mask);
    ~KeyGrab();

    KeySym key_sym;
    X11Shortcut::ModMask mod_mask;
    int ref_count = 0;
};


X11Shortcut::KeyGrab::KeyGrab(KeySym _key_sym, ModMask _mod_mask) :
    key_sym(_key_sym),
    mod_mask(_mod_mask)
{
    debug;
    //FIXME: error message, if grabbing is not possible - member: bool is_active
    XGrabKey(dpy(), XKeysymToKeycode(dpy(), key_sym),
             mod_mask, X11Application::root(), true, GrabModeAsync, GrabModeAsync);
}

X11Shortcut::KeyGrab::~KeyGrab()
{
    XUngrabKey(dpy(), XKeysymToKeycode(dpy(), key_sym),
               mod_mask, X11Application::root());
}


X11Shortcut::X11Shortcut(const char *key_sym_str, ModMask mod_mask, ShortcutSet::HandlerFunc handler_func) :
    _handler_func(handler_func)
{
    KeySym key_sym = XStringToKeysym(key_sym_str);

    _key_grab = findKeyGrab(key_sym, mod_mask);

    if (!_key_grab) {
        _key_grab = new KeyGrab(key_sym, mod_mask);
        _key_grabs.push_back(_key_grab);
    }

    _key_grab->ref_count++;

    assert(_key_grab->ref_count > 0);
}

X11Shortcut::~X11Shortcut()
{
    assert(_key_grab->ref_count > 0);

    _key_grab->ref_count--;

    assert(_key_grab->ref_count >= 0);

    if (_key_grab->ref_count == 0) {
        _key_grabs.remove(_key_grab);
        delete _key_grab;
        _key_grab = 0;
    }
}

X11Shortcut::KeyGrab *X11Shortcut::findKeyGrab(KeySym key_sym, ModMask mod_mask)
{
       for(KeyGrabList::iterator it = _key_grabs.begin();
            it != _key_grabs.end();
            it++)
        {
            KeyGrab *key_grab = *it;

            if (key_grab->key_sym == key_sym && key_grab->mod_mask == mod_mask)
                return key_grab;
        }

        return 0;
}

const X11Shortcut *X11Shortcut::findInList(const KeyGrab *grab, const List &list)
{
    for(List::const_iterator it = list.begin();
        it != list.end();
        it++)
    {
        if ((*it)->_key_grab == grab)
            return *it;
    }
    return 0;
}

bool X11Shortcut::handleKeyPress(const XKeyEvent &ev)
{
    debug;

    KeySym key_sym = XLookupKeysym(const_cast<XKeyEvent*>(&ev), 0);

    if (key_sym != NoSymbol) {
        KeyGrab *key_grab = findKeyGrab(key_sym, ev.state);
        if (key_grab) {
            X11ShortcutSet *main_shortcuts = 
                static_cast<X11ShortcutSet*>(Application::self()->mainShortcuts());

            X11ShortcutSet *active_mode_shortcuts = 
                static_cast<X11ShortcutSet*>(Application::activeWorkspace()->mode()->shortcuts());

            const X11Shortcut *shortcut = findInList(key_grab, main_shortcuts->shortcuts());
            if (shortcut) {
                shortcut->_handler_func();
                return true;
            } else {
                shortcut = findInList(key_grab, active_mode_shortcuts->shortcuts());
                if (shortcut) {
                    shortcut->_handler_func();
                    return true;
                }
            }
        }
    }

    return false;
}
