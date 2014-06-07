#ifndef __SHORTCUT_SET_H__
#define __SHORTCUT_SET_H__

#include <string>

class ShortcutSet
{
public:
    ShortcutSet(std::string name) : _name(name) {}
    virtual ~ShortcutSet() {}

    typedef void (*HandlerFunc)();

    virtual void clear() = 0;

    void createAction(std::string action_name, HandlerFunc handler_func);

private:
    // key_sequence: a backend-specific string based representation of a key sequence
    virtual void createShortcut(std::string key_sequence, HandlerFunc handler_func) = 0;

    std::string _name;
};

#endif
