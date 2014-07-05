#ifndef __ACTION_SET_H__
#define __ACTION_SET_H__

// #include "shortcut_set.h"

#include <string>

class Action;

#if 0
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

class ActionSet
{
public:
    ~ActionSet();

    void createAction(std::string action_name, int id);
    const Action *find(std::string action_name) const;
};

#endif
