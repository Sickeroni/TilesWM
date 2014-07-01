#ifndef __SHORTCUT_SET_H__
#define __SHORTCUT_SET_H__

#include <string>
#include <list>

class KeySequence;

class ShortcutSet
{
public:
    class Handler
    {
    public:
        virtual void handleShortcut(int id) = 0;
    };

    ShortcutSet(std::string name) : _name(name) {}
    ~ShortcutSet() {}

    void clear();

    void createAction(std::string action_name, int id);
    Handler *handler() const { return _handler; }
    void setHandler(Handler *handler) {
        _handler = handler;
    }

    bool handleKeyPress(const KeySequence *key_sequence) const;

private:
    struct Shortcut
    {
        Shortcut(const KeySequence *key_sequence, int id) :
            key_sequence(key_sequence), id(id) {}

        const KeySequence *key_sequence;
        int id;
    };

    std::string _name;
    Handler *_handler = 0;
    std::list<Shortcut> _shortcuts;
};

#endif
