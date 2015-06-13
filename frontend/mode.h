#ifndef __MODE_H__
#define __MODE_H__

#include "action_set.h"
#include "key_binding_set.h"

#include <string>

class WindowManager;
class Workspace;

class Mode
{
public:
    Mode(std::string name) : _name(name), _key_bindings(name, &_actions) {}
    virtual ~Mode() {}

    virtual WindowManager *createWindowManager(Workspace *workspace) = 0;

    virtual void createKeyBindings() {
        _key_bindings.createBindings();
    }
    virtual void clearKeyBindings() {
        _key_bindings.clear();
    }

    const std::string &name() const { return _name; }

    const KeyBindingSet *keyBindings() const { return &_key_bindings; }

protected:
    ActionSet _actions;

private:
    std::string _name;
    KeyBindingSet _key_bindings;
};

#endif
