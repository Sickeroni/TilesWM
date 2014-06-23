#include "main_actions.h"

#include "client_container.h"
#include "container_container.h"
#include "container_layout.h"
#include "client.h"
#include "workspace.h"
#include "application.h"
#include "container_util.h"
#include "shortcut_set.h"
#include "common.h"

using namespace ContainerUtil;

void MainActions::initShortcuts()
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

void MainActions::handleShortcut(int id)
{
    switch(id) {
        case ACTION_RELOAD_CONFIG:
            Application::self()->reloadConfig();
            break;
        case ACTION_TOGGLE_MAXIMIZE:
            {
                bool maximized = Application::activeWorkspace()->maximized();
                Application::activeWorkspace()->setMaximized(!maximized);
            }
            break;
        case ACTION_LAYOUT:
            Application::activeWorkspace()->rootContainer()->getLayout()->layoutContents();
            break;
        case ACTION_ROTATE:
            Application::activeWorkspace()->rotateOrientation();
            break;
        case ACTION_FOCUS_LEFT:
            focusSibling(LEFT);
            break;
        case ACTION_FOCUS_RIGHT:
            focusSibling(RIGHT);
            break;
        case ACTION_FOCUS_UP:
            focusSibling(UP);
            break;
        case ACTION_FOCUS_DOWN:
            focusSibling(DOWN);
            break;
        case ACTION_REDRAW:
            Application::activeWorkspace()->rootContainer()->redrawAll();
            break;
        case ACTION_FOCUS_NEXT_CLIENT:
            focusNextClient();
            break;
        case ACTION_FOCUS_PREV_CLIENT:
            focusPrevClient();
            break;
        case ACTION_RUN_PROGRAM:
            Application::runProgram("/usr/bin/gmrun");
            break;
        case ACTION_RUN_TERMINAL:
            Application::runProgram("/usr/bin/xterm");
            break;
        case ACTION_CHANGE_MODE:
            {
                size_t mode =  Application::activeWorkspace()->modeIndex();
                mode++;
                if (mode >= Application::self()->numModes())
                    mode = 0;

                Application::activeWorkspace()->setMode(mode);
            }
            break;
        case ACTION_CLOSE_ACTIVE_CLIENT:
            if (Client *c = Application::activeClient())
                c->requestClose();
            break;
        case ACTION_FOCUS_ACTIVE_CLIENT:
            Application::focusActiveClient();
        case ACTION_QUIT:
            Application::self()->requestQuit();
            break;
    }
}

void MainActions::focusPrevChild(ContainerContainer *container)
{
    if (container->activeChildIndex() != INVALID_INDEX) {
        int new_index = container->activeChildIndex() - 1;
        if (new_index >= 0) {
            container->setActiveChild(new_index);
            Application::self()->setFocus(container->activeClient());
        }
    }
}

void MainActions::focusNextChild(ContainerContainer *container)
{
    if (container->activeChildIndex() != INVALID_INDEX) {
        int new_index = container->activeChildIndex() + 1;
        if (new_index < container->numElements()) {
            container->setActiveChild(new_index);
            Application::self()->setFocus(container->activeClient());
        }
    }
}

void MainActions::focusSibling(Direction where)
{
    debug;

    ClientContainer *container = Application::activeClientContainer();
    if (!container)
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

void MainActions::focusPrevClient()
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

void MainActions::focusNextClient()
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
