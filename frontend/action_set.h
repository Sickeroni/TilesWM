#ifndef __ACTION_SET_H__
#define __ACTION_SET_H__

#include <string>
#include <vector>

class Action;
class ComplexAction;

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
    // this action set becomes owner of <complex_action>
    void createAction(std::string action_name, ComplexAction *complex_action);
    const Action *find(std::string action_name) const;

private:
    std::vector<Action*> _actions;
};

#endif
