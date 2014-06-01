#include "mode_default.h"

#include "container_container.h"
#include "client_container.h"
#include "client.h"
#include "workspace.h"
#include "shortcut_set.h"
#include "application.h"

using namespace ContainerUtil;

ModeDefault::ModeDefault() : Mode("default")
{
}


void ModeDefault::activate(Workspace *workspace)
{
    ContainerContainer *root = workspace->rootContainer();

    root->deleteEmptyChildren();
    if (ClientContainer *client_container = root->activeClientContainer()) {
        if (!client_container->isEmpty()) {
            if (!client_container->activeClient())
                client_container->setActiveChild(0);
        }
    }
}

void ModeDefault::initShortcuts()
{
    shortcuts()->createAction("moveClientLeft", &moveClientLeft);
    shortcuts()->createAction("moveClientRight", &moveClientRight);
    shortcuts()->createAction("moveClientUp", &moveClientUp);
    shortcuts()->createAction("moveClientDown", &moveClientDown);
    shortcuts()->createAction("deleteEmptyContainers", &deleteEmptyContainers);

    shortcuts()->createAction("toggleExpanding", &toggleExpanding);
    shortcuts()->createAction("toggleParentExpanding", &toggleParentExpanding);

    shortcuts()->createAction("incWidth", &incWidth);
    shortcuts()->createAction("decWidth", &decWidth);
    shortcuts()->createAction("incHeight", &incHeight);
    shortcuts()->createAction("decHeight", &decHeight);

}

void ModeDefault::tileClient(Client *client, ContainerContainer *root_container)
{
    if (!root_container->activeClientContainer()) {
        ClientContainer *c = Application::self()->createClientContainer();
        int index = root_container->addChild(c);
        root_container->setActiveChild(index);
    }

    root_container->activeClientContainer()->addChild(client);
}

void ModeDefault::moveClient(Direction direction)
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

void ModeDefault::moveClientLeft()
{
    moveClient(LEFT);
}

void ModeDefault::moveClientRight()
{
    moveClient(RIGHT);
}

void ModeDefault::moveClientUp()
{
    moveClient(UP);
}

void ModeDefault::moveClientDown()
{
    moveClient(DOWN);
}

void ModeDefault::deleteEmptyContainers()
{
    Application::activeWorkspace()->rootContainer()->deleteEmptyChildren();
}

void ModeDefault::toggleExpanding()
{
    if (ClientContainer *c = Application::activeClientContainer())
        c->enableFixedSize(!c->isFixedSize());
}

void ModeDefault::toggleParentExpanding()
{
    if (ClientContainer *c = Application::activeClientContainer())
        c->parent()->enableFixedSize(!c->parent()->isFixedSize());
}

void ModeDefault::changeSize(bool horizontal, int delta)
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

void ModeDefault::incWidth()
{
    changeSize(true, 100);
}

void ModeDefault::decWidth()
{
    changeSize(true, -100);
}

void ModeDefault::incHeight()
{
    changeSize(false, 100);
}

void ModeDefault::decHeight()
{
    changeSize(false, -100);
}
