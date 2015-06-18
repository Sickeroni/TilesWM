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
    Mode(std::string name) : _name(name), _display_name(name), _key_bindings(name, &_actions) {}
    virtual ~Mode() {}

    virtual WindowManager *createWindowManager(Workspace *workspace) = 0;

    virtual void createKeyBindings() {
        _key_bindings.createBindings();
    }
    virtual void clearKeyBindings() {
        _key_bindings.clear();
    }

    const std::string &name() const { return _name; }
    const std::string &displayName() const  { return _display_name; }

    const KeyBindingSet *keyBindings() const { return &_key_bindings; }

protected:
    void setDisplayName(std::string name) {
        _display_name = name;
    }

    ActionSet _actions;

private:
    std::string _name, _display_name;
    KeyBindingSet _key_bindings;
};

#endif
