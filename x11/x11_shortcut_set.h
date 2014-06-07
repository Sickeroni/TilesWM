#ifndef __X11_SHORTCUT_SET_H__
#define __X11_SHORTCUT_SET_H__


#include "shortcut_set.h"
#include "x11_shortcut.h"

class X11ShortcutSet final : public ShortcutSet
{
    X11Shortcut::List _shortcuts;

public:
    X11ShortcutSet(std::string name);
    ~X11ShortcutSet();

    virtual void createShortcut(std::string key_sequence, HandlerFunc handler_func) override;

    const X11Shortcut::List &shortcuts() const { return _shortcuts; }
};


#endif // __X11_SHORTCUT_SET_H__
