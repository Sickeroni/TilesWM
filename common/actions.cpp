#include "actions.h"

#include "client_container.h"
#include "container_container.h"
#include "container_layout.h"
#include "client.h"
#include "workspace.h"
#include "application.h"
#include "container_util.h"
#include "common.h"

using namespace ContainerUtil;

namespace Actions
{


void focusPrevChild(ContainerContainer *container)
{
    if (container->activeChildIndex() != INVALID_INDEX) {
        int new_index = container->activeChildIndex() - 1;
        if (new_index >= 0) {
            container->setActiveChild(new_index);
            if (container->activeClient())
                Application::self()->setFocus(container->activeClient());
            else
                Application::self()->setFocus(0);
        }
    }
}

void focusNextChild(ContainerContainer *container)
{
    if (container->activeChildIndex() != INVALID_INDEX) {
        int new_index = container->activeChildIndex() + 1;
        if (new_index < container->numElements()) {
            container->setActiveChild(new_index);
            if (container->activeClient())
                Application::self()->setFocus(container->activeClient());
            else
                Application::self()->setFocus(0);
        }
    }
}

void focusSibling(Direction where)
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

void focusLeft()
{
    focusSibling(LEFT);
}

void focusRight()
{
    focusSibling(RIGHT);
}

void focusUp()
{
    focusSibling(UP);
}

void focusDown()
{
    focusSibling(DOWN);
}

void focusPrevClient()
{
    ClientContainer *container = Application::activeClientContainer();
    if(container) {
        int index = container->activeChildIndex();
        if (index != INVALID_INDEX) {
            index--;
            if (index >= 0) {
                container->setActiveChild(index);
                Application::self()->setFocus(container->child(index));
            }
        }
    }
}

void focusNextClient()
{
    ClientContainer *container = Application::activeClientContainer();
    if(container) {
        int index = container->activeChildIndex();
        if (index != INVALID_INDEX) {
            index++;
            if (index < container->numElements()) {
                container->setActiveChild(index);
                Application::self()->setFocus(container->child(index));
            }
        }
    }
}

void runTerminal()
{
    Application::runProgram("/usr/bin/xterm");
}

void runProgram()
{
    Application::runProgram("/usr/bin/gmrun");
}

void toggleMaximize()
{
    debug;
    bool maximized = Application::activeWorkspace()->maximized();
    Application::activeWorkspace()->setMaximized(!maximized);
}

void rotate()
{
    Container::rotateOrientation();
    Application::activeWorkspace()->rootContainer()->getLayout()->layoutContents();
}

void layout()
{
    debug;
    Application::activeWorkspace()->rootContainer()->getLayout()->layoutContents();
}

void redraw()
{
    debug;
    Application::activeWorkspace()->rootContainer()->redrawAll();
}

void changeMode()
{
    debug;
    size_t mode =  Application::activeWorkspace()->modeIndex();
    mode++;
    if (mode >= Application::self()->numModes())
        mode = 0;

    Application::activeWorkspace()->setMode(mode);
}

void closeActiveClient()
{
    Client *c = Application::activeClient();
    if (c)
        c->requestClose();
}

void quit()
{
    Application::self()->requestQuit();
}


} // namespace Actions
