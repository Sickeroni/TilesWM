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

    deleteEmptyChildren(root);
    if (ClientContainer *client_container = root->activeClientContainer()) {
        if (!client_container->isEmpty()) {
            if (!client_container->activeClient())
                client_container->setActiveChild(0);
        }
    }
}

void ModeDefault::initShortcuts()
{
    createAction("moveClientLeft", ACTION_MOVE_CLIENT_LEFT);
    createAction("moveClientRight", ACTION_MOVE_CLIENT_RIGHT);
    createAction("moveClientUp", ACTION_MOVE_CLIENT_UP);
    createAction("moveClientDown", ACTION_MOVE_CLIENT_DOWN);
    createAction("deleteEmptyContainers", ACTION_DELETE_EMPTY_CONTAINERS);

    createAction("toggleExpanding", ACTION_TOGGLE_EXPANDING);
    createAction("toggleParentExpanding", ACTION_TOGGLE_PARENT_EXPANDING);

    createAction("incWidth", ACTION_INC_WIDTH);
    createAction("decWidth", ACTION_DEC_WIDTH);
    createAction("incHeight", ACTION_INC_HEIGHT);
    createAction("decHeight", ACTION_DEC_HEIGHT);

    createAction("setFixedSizeToMinimum", ACTION_SET_FIXED_SIZE_TO_MINIMUM);
}

void ModeDefault::handleShortcut(int id)
{
    switch(id) {
        case ACTION_MOVE_CLIENT_LEFT:
            moveClient(LEFT);
            break;
        case ACTION_MOVE_CLIENT_RIGHT:
            moveClient(RIGHT);
            break;
        case ACTION_MOVE_CLIENT_UP:
            moveClient(UP);
            break;
        case ACTION_MOVE_CLIENT_DOWN:
            moveClient(DOWN);
            break;
        case ACTION_DELETE_EMPTY_CONTAINERS:
            deleteEmptyChildren(Application::activeWorkspace()->rootContainer());
            break;
        case ACTION_TOGGLE_EXPANDING:
            if (ClientContainer *c = Application::activeClientContainer())
                c->enableFixedSize(!c->isFixedSize());
            break;
        case ACTION_TOGGLE_PARENT_EXPANDING:
            if (ClientContainer *c = Application::activeClientContainer())
                c->parent()->enableFixedSize(!c->parent()->isFixedSize());
            break;
        case ACTION_INC_WIDTH:
            changeSize(true, -100);
            break;
        case ACTION_DEC_WIDTH:
            changeSize(true, 100);
            break;
        case ACTION_INC_HEIGHT:
            changeSize(false, 100);
            break;
        case ACTION_DEC_HEIGHT:
            changeSize(false, -100);
            break;
        case ACTION_SET_FIXED_SIZE_TO_MINIMUM:
            setFixedSizeToMinimum();
            break;
    }
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

void ModeDefault::setFixedSizeToMinimum()
{
    if (ClientContainer *container = Application::activeClientContainer()) {
        if (Client *client = container->activeClient()) {
            container->setFixedWidth(client->minWidth());
            container->setFixedHeight(client->minHeight());
            container->enableFixedSize(true);
        }
    }
}
