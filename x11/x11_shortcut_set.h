#ifndef __X11_SHORTCUT_SET_H__
#define __X11_SHORTCUT_SET_H__

#include "x11_shortcut.h"


class X11ShortcutSet
{
    std::vector<X11Shortcut*> _shortcuts;

protected:
    void createShortcut(const char *key_sym, X11Shortcut::ModMask mod_mask,
                        X11Shortcut::HandlerFunc handler_func);

public:
    virtual ~X11ShortcutSet();
};


#endif // __X11_SHORTCUT_SET_H__
