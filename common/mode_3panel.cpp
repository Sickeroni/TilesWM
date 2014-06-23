#include "mode_3panel.h"

#include "shortcut_set.h"
#include "container_container.h"
#include "container_util.h"
#include "client_container.h"
#include "client.h"
#include "workspace.h"
#include "application.h"
#include "common.h"

using namespace ContainerUtil;

Mode3Panel::Mode3Panel() : Mode("3panel")
{
}

void Mode3Panel::activate(Workspace *workspace)
{
    ContainerContainer *root = workspace->rootContainer();

    std::vector<Client*> clients;
    ContainerUtil::emptyContainer(root, clients);

    assert(root->isEmpty());

    ContainerContainer *child = Application::self()->createContainerContainer();

    child->addChild(Application::self()->createClientContainer());
    child->addChild(Application::self()->createClientContainer());
    child->setActiveChild(0);

    root->addChild(Application::self()->createClientContainer());
    root->addChild(child);
    root->setActiveChild(0);

    for(size_t i = 0; i < clients.size(); i++)
        tileClient(clients[i], root);

    if (!root->activeClientContainer()->isEmpty())
        root->activeClientContainer()->setActiveChild(0);
}

void Mode3Panel::initShortcuts()
{
    createAction("moveClientLeft", ACTION_MOVE_CLIENT_LEFT);
    createAction("moveClientRight", ACTION_MOVE_CLIENT_RIGHT);
    createAction("moveClientUp", ACTION_MOVE_CLIENT_UP);
    createAction("moveClientDown", ACTION_MOVE_CLIENT_DOWN);

    createAction("moveHSplitLeft", ACTION_MOVE_HSPLIT_LEFT);
    createAction("moveHSplitRight", ACTION_MOVE_HSPLIT_RIGHT);
    createAction("moveVSplitUp", ACTION_MOVE_HSPLIT_UP);
    createAction("moveVSplitDown", ACTION_MOVE_HSPLIT_DOWN);

    createAction("togglePrimaryExpanding", ACTION_TOGGLE_PRIMARY_EXPANDING);
    createAction("toggleSecondaryExpanding", ACTION_TOGGLE_SECONDARY_EXPANDING);

    createAction("setPrimarySlaveToMinimum", ACTION_SET_PRIMARY_SLAVE_TO_MINIMUM);
    createAction("setSecondarySlaveToMinimum", ACTION_SET_SECONDARY_SLAVE_TO_MINIMUM);
}

void Mode3Panel::handleShortcut(int id)
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
        case ACTION_MOVE_HSPLIT_RIGHT:
            moveSplitter(true, 100);
            break;
        case ACTION_MOVE_HSPLIT_LEFT:
            moveSplitter(true, -100);
            break;
        case ACTION_MOVE_HSPLIT_UP:
            moveSplitter(false, 100);
            break;
        case ACTION_MOVE_HSPLIT_DOWN:
            moveSplitter(false, -100);
            break;
        case ACTION_TOGGLE_PRIMARY_EXPANDING:
            togglePrimaryExpanding();
            break;
        case ACTION_TOGGLE_SECONDARY_EXPANDING:
            toggleSecondaryExpanding();
            break;
        case ACTION_SET_PRIMARY_SLAVE_TO_MINIMUM:
            setPrimarySlaveToMinimum();
            break;
        case ACTION_SET_SECONDARY_SLAVE_TO_MINIMUM:
            setSecondarySlaveToMinimum();
            break;
    }
}


void Mode3Panel::tileClient(Client *client, ContainerContainer *root_container)
{
    assert(root_container->activeClientContainer());

    root_container->activeClientContainer()->addChild(client);
}

void Mode3Panel::moveClient(Direction direction)
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
        if (orientationOfDirection(direction) == container->parent()->orientation())
            target = getSibling(container, backward, false);
        else
            target = getSibling(container->parent(), backward, false);
    }

    if (target) {
        container->removeChild(client);
        int index = target->addChild(client);
        target->setActiveChild(index);
        target->makeActive();
        Application::self()->setFocus(client);
    }
}

void Mode3Panel::moveSplitter(bool horizontal, int delta)
{
    ContainerContainer *root = Application::activeWorkspace()->rootContainer();

    if (horizontal && root->isHorizontal()) {
        Container *c =  root->child(1);
        c->setFixedWidth(c->fixedWidth() - delta);
    } else if (!horizontal && root->isHorizontal()) {
        Container *c = root->child(1)->toContainerContainer()->child(1);
        c->setFixedHeight(c->fixedHeight() - delta);
    }
}

void Mode3Panel::togglePrimaryExpanding()
{
    ContainerContainer *root = Application::activeWorkspace()->rootContainer();


    Container *c = root->child(1);
    c->enableFixedSize(!c->isFixedSize());
}

void Mode3Panel::toggleSecondaryExpanding()
{
    ContainerContainer *root = Application::activeWorkspace()->rootContainer();

    Container *c = root->child(1)->toContainerContainer()->child(1);
    c->enableFixedSize(!c->isFixedSize());
}

void Mode3Panel::setPrimarySlaveToMinimum()
{
    ContainerContainer *root = Application::activeWorkspace()->rootContainer();

    ContainerContainer *container = root->child(1)->toContainerContainer();
    if (Client *client = container->child(0)->toClientContainer()->activeClient()) {
        container->setFixedWidth(client->minWidth());
        container->setFixedHeight(client->minHeight());
        container->enableFixedSize(true);
    }
}

void Mode3Panel::setSecondarySlaveToMinimum()
{
    ContainerContainer *root = Application::activeWorkspace()->rootContainer();

    ClientContainer *container = root->child(1)->toContainerContainer()->child(1)->toClientContainer();
    if (Client *client = container->activeClient()) {
        container->setFixedWidth(client->minWidth());
        container->setFixedHeight(client->minHeight());
        container->enableFixedSize(true);
    }
}
