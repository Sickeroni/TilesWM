#ifndef __X11_KEY_HANDLER_H__
#define __X11_KEY_HANDLER_H__

#include "x11_shortcut.h"


class X11KeyHandler
{
    std::vector<X11Shortcut*> _shortcuts;

protected:
    void createShortcut(const char *key_sym, X11Shortcut::ModMask mod_mask,
                        X11Shortcut::HandlerFunc handler_func);

public:
    ~X11KeyHandler();
};


#endif // __X11_KEY_HANDLER_H__
