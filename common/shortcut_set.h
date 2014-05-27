#ifndef __SHORTCUT_SET_H__
#define __SHORTCUT_SET_H__

#include <string>

class ShortcutSet
{
public:
    virtual ~ShortcutSet() {}

    typedef void (*HandlerFunc)();

    // key_sequence: a backend-specific string based representation of a key sequence
    virtual void createShortcut(std::string key_sequence, HandlerFunc handler_func) = 0;
};

#endif
