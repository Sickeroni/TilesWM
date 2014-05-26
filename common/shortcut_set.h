#ifndef __SHORTCUT_SET_H__
#define __SHORTCUT_SET_H__

class ShortcutSet
{
public:
    virtual ~ShortcutSet() {}

    typedef void (*HandlerFunc)();

    virtual void createShortcut(const char *key_sym, unsigned int mod_mask,
                                HandlerFunc handler_func) = 0;

};

#endif
