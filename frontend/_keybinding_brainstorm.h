#ifndef __ACTION_SET_H__
#define __ACTION_SET_H__

#include "shortcut_set.h"

#include <string>


class KeySequenceHandler
{
public:
    void handleKeySequence(const KeySequence *sequence);

protected:
    virtual void performAction(int id) = 0;
    virtual void performComplexAction(ComplexAction *action, ActionParameters *parameters) = 0;
    virtual const KeyBindingSet *keyBindings() = 0;

private:
    // the window manager this handler is associated with
    WindowManager *_wm = 0;
};

class WindowManager : public KeySequenceHandler
{
    virtual void performComplexAction(ComplexAction *action, ActionParameters *parameters) override {
        dynamic_cast<ComplexWindowManagerAction*>(action)->execute(this, parameters);
    }
};


void KeySequenceHandler::handleKeySequence(KeySequence *sequence)
{
    const KeyBinding *binding = keyBindings()->find(sequence);
    if (binding) {
        switch (binding->action->type()) {
            case TYPE_SIMPLE:
                performAction(static_cast<SimpleAction*>(binding->action)->id());
                break;
            case TYPE_COMPLEX:
                performComplexAction(static_cast<ComplexAction*>(binding->action), binding->parameters);
                break;
        }
    }
}


struct ActionParameters
{
    virtual ~ActionParameters() {}
};


class Action
{
    enum Type
    {
        TYPE_SIMPLE,
        TYPE_COMPLEX
    };

    string name;

    Type type;
};


struct Action
{
    enum Type
    {
        TYPE_SIMPLE,
        TYPE_COMPLEX
    };

    string name;

    Type type = TYPE_SIMPLE;

    union {
        ComplexAction *complex_action = 0;
        int id = -1;
    };
};


class SimpleAction
{
public:
//     struct Handler
//     {
//         virtual void performAction(int id, WindowManager *wm) = 0;
//     };

    int _id;
//     Handler *_handler;
};



class ComplexAction
{
public:
    virtual ActionParameters *parseParameters(const vector<string&> &parameters) { return 0; }
};




class ActionSet
{
    void addAction(std::string name, int id) {
        _actions.push_back(new SimpleAction(name, id));
    }

private:
    list<Action*> _actions;
};

#endif
