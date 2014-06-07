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
    createAction("toggleMaximize", &toggleMaximize);
    createAction("layout", &layout);
    createAction("rotate", &rotate);
    createAction("focusLeft", &focusLeft);
    createAction("focusRight", &focusRight);
    createAction("focusUp", &focusUp);
    createAction("focusDown", &focusDown);
    createAction("redraw", &redraw);
    createAction("focusPrevClient", &focusPrevClient);
    createAction("focusNextClient", &focusNextClient);
    createAction("runProgram", &runProgram);
    createAction("runTerminal", &runTerminal);
    createAction("changeMode", &changeMode);
    createAction("closeActiveClient", &closeActiveClient);
    createAction("quit", &quit);

    createAction("focusActiveClient", &Application::focusActiveClient);
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

void MainActions::focusLeft()
{
    focusSibling(LEFT);
}

void MainActions::focusRight()
{
    focusSibling(RIGHT);
}

void MainActions::focusUp()
{
    focusSibling(UP);
}

void MainActions::focusDown()
{
    focusSibling(DOWN);
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

void MainActions::runTerminal()
{
    Application::runProgram("/usr/bin/xterm");
}

void MainActions::runProgram()
{
    Application::runProgram("/usr/bin/gmrun");
}

void MainActions::toggleMaximize()
{
    debug;
    bool maximized = Application::activeWorkspace()->maximized();
    Application::activeWorkspace()->setMaximized(!maximized);
}

void MainActions::rotate()
{
    Application::activeWorkspace()->rotateOrientation();
}

void MainActions::layout()
{
    debug;
    Application::activeWorkspace()->rootContainer()->getLayout()->layoutContents();
}

void MainActions::redraw()
{
    debug;
    Application::activeWorkspace()->rootContainer()->redrawAll();
}

void MainActions::changeMode()
{
    debug;
    size_t mode =  Application::activeWorkspace()->modeIndex();
    mode++;
    if (mode >= Application::self()->numModes())
        mode = 0;

    Application::activeWorkspace()->setMode(mode);
}

void MainActions::closeActiveClient()
{
    Client *c = Application::activeClient();
    if (c)
        c->requestClose();
}

void MainActions::quit()
{
    Application::self()->requestQuit();
}
