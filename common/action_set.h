#ifndef __ACTION_SET_H__
#define __ACTION_SET_H__

#include <string>

class ShortcutSet;

class ActionSet
{
public:
    ActionSet(std::string name);
    virtual ~ActionSet();

    virtual void initShortcuts() = 0;
    ShortcutSet *shortcuts() { return _shortcuts; }

private:
    ShortcutSet *_shortcuts;
};

#endif
