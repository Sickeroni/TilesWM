#include "common_actions.h"

#include "common_complex_action.h"
#include "action_run.h"
#include "application.h"
#include "workspace.h"
#include "window_manager.h"
#include "client_wrapper.h"
#include "backend.h"
#include "common.h"

CommonActions::CommonActions() :
    _key_bindings("common", &_actions)
{
    _actions.createAction("reloadConfig", ACTION_RELOAD_CONFIG);
    _actions.createAction("toggleMaximize", ACTION_TOGGLE_MAXIMIZE);
    _actions.createAction("layout", ACTION_LAYOUT);
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
    _actions.createAction("toggleDecoration", ACTION_TOGGLE_DECORATION);
    _actions.createAction("updateClientGeometry", ACTION_UPDATE_CLIENT_GEOMETRY);
    _actions.createAction("quit", ACTION_QUIT);
    _actions.createAction("run", new ActionRun());
}

CommonActions::~CommonActions()
{
    _key_bindings.clear();
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
            Application::activeWorkspace()->redrawAll();
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
            if (ClientWrapper *c = Application::activeClient()) {
                c->requestClose();
            }
            break;
        case ACTION_FOCUS_ACTIVE_CLIENT:
            Application::self()->focusActiveClient();
            break;
#if 0
        case ACTION_TOGGLE_DECORATION:
            if (Client *client = Application::self()->activeClient()) {
                client->setHasDecoration(!client->hasDecoration());
            }
            break;
        case ACTION_UPDATE_CLIENT_GEOMETRY:
            if (Client *client = Application::self()->activeClient()) {
                client->updateFrameGeometry();
            }
            break;
#endif
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
