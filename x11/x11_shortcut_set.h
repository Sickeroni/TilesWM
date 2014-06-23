#ifndef __X11_SHORTCUT_SET_H__
#define __X11_SHORTCUT_SET_H__


#include "shortcut_set.h"
#include "x11_global.h"

#include <list>

class X11ShortcutSet final : public ShortcutSet
{
public:

    X11ShortcutSet(std::string name);
    ~X11ShortcutSet();

    virtual void clear() override;

    bool handleKeyPress(const X11Global::KeySequence &key_sequence) const;

private:
    struct Shortcut
    {
        Shortcut(const X11Global::KeySequence &key_sequence, HandlerFunc handler_func) :
            key_sequence(key_sequence), handler_func(handler_func) {}

        X11Global::KeySequence key_sequence;
        HandlerFunc handler_func = 0;
    };

    virtual void createShortcut(std::string key_sequence, HandlerFunc handler_func) override;

    std::list<Shortcut> _shortcuts;
};


#endif // __X11_SHORTCUT_SET_H__
