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


void moveClient(Direction direction)
{

    ClientContainer *container = Application::activeClientContainer();
    if (!container)
        return;

    assert(container->workspace() == Application::activeWorkspace());
    assert(container->parent());

    Client *client = container->activeClient();
    if (!client)
        return;

    assert(client->container() == container);

    if (container->workspace()->maximized()) //FIXME
        return;

    if (!client->isMapped()) //FIXME is this ok ?
        return;

    bool backward = !isForwardDirection(direction);

    ClientContainer *target = 0;

    if (container->parent()) {
        if (orientationOfDirection(direction) == container->parent()->orientation()) { // easy case
            if (container->numElements() > 1) // only create new direct sibling if container doesn't become empty
                target = getSibling(container, backward, true);
            else
                target = getSibling(container, backward, false);
        }

        else { // difficult case:
                 // if client container becomes empty -> use sibling of parent container;
                 // else: 1. replace this with new parent container; 2. add this and new client container to parent created in step 1

            if (container->numElements() <= 1) // cant't split - use sibling of parent container
                target = getSibling(container->parent(), backward, true);
            else { // split this
                target = splitContainer(container, backward);

                if (!target) { // split failed - maximum hierarchy depth exceeded ?
                    target = getSibling(container->parent(), backward, true);
                }
            }
        }
    }

    if (target) {
        container->removeChild(client);
        int index = target->addChild(client);
        target->setActiveChild(index);
        target->makeActive();
        Application::self()->setFocus(client);
    }
}

void moveClientLeft()
{
    moveClient(LEFT);
}

void moveClientRight()
{
    moveClient(RIGHT);
}

void moveClientUp()
{
    moveClient(UP);
}

void moveClientDown()
{
    moveClient(DOWN);
}

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

void deleteEmptyContainers()
{
    Application::activeWorkspace()->rootContainer()->deleteEmptyChildren();
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

void toggleExpanding()
{
    if (ClientContainer *c = Application::activeClientContainer())
        c->enableFixedSize(!c->isFixedSize());
}

void toggleParentExpanding()
{
    if (ClientContainer *c = Application::activeClientContainer())
        c->parent()->enableFixedSize(!c->parent()->isFixedSize());
}

void incExtraSpace()
{
//         Application::activeClientContainer()->parent()->incAvailableSpacePortion(Application::activeClientContainer(), 100);
}

void decExtraSpace()
{
//         Application::activeClientContainer()->parent()->decAvailableSpacePortion(Application::activeClientContainer(), 100);
//         Application::activeClientContainer()->decCustomSize();
}

void resetExtraSpace()
{
//         Application::activeClientContainer()->setCustomSizeActive(false);
}

void changeSize(bool horizontal, int delta)
{
    if (Container *c = Application::activeClientContainer()) {
        Container *target = (c->parent()->isHorizontal() == horizontal) ? c : c->parent();
        if (target->isFixedSize()) {
            if (horizontal)
                target->setFixedWidth(target->fixedWidth() + delta);
            else
                target->setFixedHeight(target->fixedHeight() + delta);
        }
    }
}

void incWidth()
{
    changeSize(true, 100);
}

void decWidth()
{
    changeSize(true, -100);
}

void incHeight()
{
    changeSize(false, 100);
}

void decHeight()
{
    changeSize(false, -100);
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

void quit()
{
    Application::self()->requestQuit();
}

} // namespace Actions
