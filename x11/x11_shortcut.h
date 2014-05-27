#ifndef __X11_SHORTCUT_H__
#define __X11_SHORTCUT_H__

#include "shortcut_set.h"

#include <X11/Xlib.h>

#include <list>
#include <string>

class X11Shortcut
{
public:
    typedef unsigned int ModMask;
    typedef std::list<X11Shortcut*> List;

    X11Shortcut(KeySym key_sym, ModMask mod_mask, ShortcutSet::HandlerFunc handler_func);
    ~X11Shortcut();

    static bool handleKeyPress(const XKeyEvent &ev);

private:
    struct KeyGrab;
    typedef std::list<KeyGrab*> KeyGrabList;

    static const X11Shortcut *findInList(const KeyGrab *grab, const List &list);
    static KeyGrab *findKeyGrab(KeySym key_sym, ModMask mod_mask);

    KeyGrab *_key_grab = 0;
    ShortcutSet::HandlerFunc _handler_func = 0;

    static KeyGrabList _key_grabs;
};


#endif // __X11_SHORTCUT_H__
