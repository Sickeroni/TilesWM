#ifndef __ACTION_SET_H__
#define __ACTION_SET_H__

#include "shortcut_set.h"

#include <string>

class ActionSet : public ShortcutSet::Handler
{
public:
    ActionSet(std::string name);
    virtual ~ActionSet();

    virtual void initShortcuts() = 0;

    const ShortcutSet *shortcuts() { return _shortcuts; }
    void reloadShortcuts();

protected:
    void createAction(std::string action_name, int id) {
        _shortcuts->createAction(action_name, id);
    }

private:
    ShortcutSet *_shortcuts;
};

#endif
