#ifndef __X11_SHORTCUT_H__
#define __X11_SHORTCUT_H__

#include <X11/Xlib.h>

#include <vector>
#include <map>


class X11Shortcut
{
public:
    typedef std::vector<X11Shortcut*> ShortcutList;
    typedef std::map<KeySym, ShortcutList> KeySymMapping;
    typedef unsigned int ModMask;
    typedef void (*HandlerFunc)();

    X11Shortcut(const char *key_sym, ModMask mod_mask, HandlerFunc handler_func);
    ~X11Shortcut();

    static bool handleKeyPress(const XKeyEvent &ev);

private:
    KeySym _key_sym;
    ModMask _mod_mask;
    HandlerFunc _handler_func;


    static KeySymMapping _key_sym_index;
};


#endif // __X11_SHORTCUT_H__
