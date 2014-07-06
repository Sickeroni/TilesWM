#include "common_actions.h"

#include "common_complex_action.h"
#include "action_run.h"
#include "application.h"
#include "key_grab_set.h"
#include "workspace.h"
#include "window_manager.h"
// #include "client.h"
#include "backend.h"
#include "common.h"

CommonActions::CommonActions() :
    _key_bindings("keybindings.common", &_actions)
{
    _actions.createAction("reloadConfig", ACTION_RELOAD_CONFIG);
    _actions.createAction("toggleMaximize", ACTION_TOGGLE_MAXIMIZE);
    _actions.createAction("layout", ACTION_LAYOUT);
    _actions.createAction("rotate", ACTION_ROTATE);
    _actions.createAction("focusLeft", ACTION_FOCUS_LEFT);
    _actions.createAction("focusRight", ACTION_FOCUS_RIGHT);
    _actions.createAction("focusUp", ACTION_FOCUS_UP);
    _actions.createAction("focusDown", ACTION_FOCUS_DOWN);
    _actions.createAction("redraw", ACTION_REDRAW);
    _actions.createAction("focusPrevClient", ACTION_FOCUS_PREV_CLIENT);
    _actions.createAction("focusNextClient", ACTION_FOCUS_NEXT_CLIENT);
    _actions.createAction("runProgram", ACTION_RUN_PROGRAM);
    _actions.createAction("runTerminal", ACTION_RUN_TERMINAL);
    _actions.createAction("changeMode", ACTION_CHANGE_MODE);
    _actions.createAction("closeActiveClient", ACTION_CLOSE_ACTIVE_CLIENT);
    _actions.createAction("focusActiveClient", ACTION_FOCUS_ACTIVE_CLIENT);
    _actions.createAction("quit", ACTION_QUIT);
    _actions.createAction("run", new ActionRun());
}

CommonActions::~CommonActions()
{
}

void CommonActions::createKeyBindings()
{
    _key_bindings.createBindings();
}

void CommonActions::performAction(int id)
{
    switch(id) {
        case ACTION_RELOAD_CONFIG:
            Application::self()->reloadConfig();
            break;
        case ACTION_TOGGLE_MAXIMIZE:
            assert(0);
            {
//                 bool maximized = Application::activeWorkspace()->maximized();
//                 Application::activeWorkspace()->setMaximized(!maximized);
            }
            break;
        case ACTION_LAYOUT:
            Application::activeWorkspace()->windowManager()->layout();
            break;
        case ACTION_REDRAW:
            assert(0);
//             Application::activeWorkspace()->rootContainer()->redrawAll();
            break;
        case ACTION_RUN_PROGRAM:
            Application::runProgram("/usr/bin/gmrun");
            break;
        case ACTION_RUN_TERMINAL:
            Application::runProgram("/usr/bin/xterm");
            break;
        case ACTION_CHANGE_MODE:
            assert(0);
//             {
//                 size_t mode = Application::activeWorkspace()->modeIndex();
//                 mode++;
//                 if (mode >= Application::self()->numModes())
//                     mode = 0;
//
//                 Application::activeWorkspace()->setMode(mode);
//             }
            break;
        case ACTION_CLOSE_ACTIVE_CLIENT:
            assert(0);
//             if (Widget *c = Application::activeClient()) {
//                 c->toClient()->requestClose();
//             }
            break;
        case ACTION_FOCUS_ACTIVE_CLIENT:
            Application::self()->focusActiveClient();
            break;
        case ACTION_QUIT:
            Application::self()->backend()->requestQuit();
            break;
    }
}

void CommonActions::performComplexAction(const ComplexAction *action, const ComplexAction::Parameters *parameters)
{
    const CommonComplexAction *a = dynamic_cast<const CommonComplexAction*>(action);
    a->exec(parameters);
}

const KeyBindingSet *CommonActions::keyBindings()
{
    return &_key_bindings;
}

#if 0
void CommonActions::initShortcuts()
{
    createAction("reloadConfig", ACTION_RELOAD_CONFIG);
    createAction("toggleMaximize", ACTION_TOGGLE_MAXIMIZE);
    createAction("layout", ACTION_LAYOUT);
    createAction("rotate", ACTION_ROTATE);
    createAction("focusLeft", ACTION_FOCUS_LEFT);
    createAction("focusRight", ACTION_FOCUS_RIGHT);
    createAction("focusUp", ACTION_FOCUS_UP);
    createAction("focusDown", ACTION_FOCUS_DOWN);
    createAction("redraw", ACTION_REDRAW);
    createAction("focusPrevClient", ACTION_FOCUS_PREV_CLIENT);
    createAction("focusNextClient", ACTION_FOCUS_NEXT_CLIENT);
    createAction("runProgram", ACTION_RUN_PROGRAM);
    createAction("runTerminal", ACTION_RUN_TERMINAL);
    createAction("changeMode", ACTION_CHANGE_MODE);
    createAction("closeActiveClient", ACTION_CLOSE_ACTIVE_CLIENT);
    createAction("focusActiveClient", ACTION_FOCUS_ACTIVE_CLIENT);
    createAction("quit", ACTION_QUIT);
}
#endif

#if 0
void CommonActions::handleShortcut(int id)
{
    switch(id) {
        case ACTION_RELOAD_CONFIG:
            Application::self()->reloadConfig();
            break;
        case ACTION_TOGGLE_MAXIMIZE:
            {
//                 bool maximized = Application::activeWorkspace()->maximized();
//                 Application::activeWorkspace()->setMaximized(!maximized);
            }
            break;
        case ACTION_LAYOUT:
            Application::activeWorkspace()->windowManager()->layout();
            break;
        case ACTION_ROTATE:
//             Application::activeWorkspace()->rotateOrientation();
            break;
        case ACTION_FOCUS_LEFT:
//             focusSibling(LEFT);
            break;
        case ACTION_FOCUS_RIGHT:
//             focusSibling(RIGHT);
            break;
        case ACTION_FOCUS_UP:
//             focusSibling(UP);
            break;
        case ACTION_FOCUS_DOWN:
//             focusSibling(DOWN);
            break;
        case ACTION_REDRAW:
//             Application::activeWorkspace()->rootContainer()->redrawAll();
            break;
        case ACTION_FOCUS_NEXT_CLIENT:
//             focusNextClient();
            break;
        case ACTION_FOCUS_PREV_CLIENT:
//             focusPrevClient();
            break;
        case ACTION_RUN_PROGRAM:
            Application::runProgram("/usr/bin/gmrun");
            break;
        case ACTION_RUN_TERMINAL:
            Application::runProgram("/usr/bin/xterm");
            break;
        case ACTION_CHANGE_MODE:
            {
                size_t mode = Application::activeWorkspace()->modeIndex();
                mode++;
                if (mode >= Application::self()->numModes())
                    mode = 0;

                Application::activeWorkspace()->setMode(mode);
            }
            break;
        case ACTION_CLOSE_ACTIVE_CLIENT:
            if (Widget *c = Application::activeClient()) {
                c->toClient()->requestClose();
            }
            break;
        case ACTION_FOCUS_ACTIVE_CLIENT:
            Application::focusActiveClient();
            break;
        case ACTION_QUIT:
            Application::self()->requestQuit();
            break;
    }
}

#if 0
void CommonActions::focusPrevChild(ContainerContainer *container)
{
    if (container->activeChildIndex() != INVALID_INDEX) {
        int new_index = container->activeChildIndex() - 1;
        if (new_index >= 0) {
            container->setActiveChild(new_index);
            Application::self()->setFocus(container->activeClient());
        }
    }
}

void CommonActions::focusNextChild(ContainerContainer *container)
{
    if (container->activeChildIndex() != INVALID_INDEX) {
        int new_index = container->activeChildIndex() + 1;
        if (new_index < container->numElements()) {
            container->setActiveChild(new_index);
            Application::self()->setFocus(container->activeClient());
        }
    }
}

void CommonActions::focusSibling(Direction where)
{
    debug;

    ClientContainer *container = Application::activeClientContainer();
    if (!container)
        return;

    if (!container->parent())
        return;

    ContainerContainer *parent = 0;
    if (orientationOfDirection(where) == container->parent()->orientation())
        parent = container->parent();
    else
        parent = container->parent()->parent();

    if (parent) {
        if (isForwardDirection(where))
            focusNextChild(parent);
        else
            focusPrevChild(parent);
    }
}

void CommonActions::focusPrevClient()
{
    ClientContainer *container = Application::activeClientContainer();
    if(container) {
        int index = container->activeChildIndex();
        if (index != INVALID_INDEX) {
            index--;
            if (index >= 0) {
                container->setActiveChild(index);
                Application::self()->setFocus(container->activeClient());
            }
        }
    }
}

void CommonActions::focusNextClient()
{
    ClientContainer *container = Application::activeClientContainer();
    if(container) {
        int index = container->activeChildIndex();
        if (index != INVALID_INDEX) {
            index++;
            if (index < container->numElements()) {
                container->setActiveChild(index);
                Application::self()->setFocus(container->activeClient());
            }
        }
    }
}
#endif
#endif
