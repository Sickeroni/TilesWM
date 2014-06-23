#ifndef __SHORTCUT_SET_H__
#define __SHORTCUT_SET_H__

#include <string>

class ShortcutSet
{
public:
    class Handler
    {
    public:
        virtual void handleShortcut(int id) = 0;
    };

    ShortcutSet(std::string name) : _name(name) {}
    virtual ~ShortcutSet() {}

    virtual void clear() = 0;

    void createAction(std::string action_name, int id);
    Handler *handler() const { return _handler; }
    void setHandler(Handler *handler) {
        _handler = handler;
    }

private:
    // key_sequence: a backend-specific string based representation of a key sequence
    virtual void createShortcut(std::string key_sequence, int id) = 0;

    std::string _name;
    Handler *_handler = 0;
};

#endif
