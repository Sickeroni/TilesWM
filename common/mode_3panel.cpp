#include "mode_3panel.h"

#include "actions.h"
#include "shortcut_set.h"
#include "container_container.h"
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
    assert(root->isEmpty());

    ContainerContainer *child = Application::self()->createContainerContainer();

    child->addChild(Application::self()->createClientContainer());
    child->addChild(Application::self()->createClientContainer());
    child->setActiveChild(0);

    root->addChild(Application::self()->createClientContainer());
    root->addChild(child);

    root->setActiveChild(0);
}

void Mode3Panel::initShortcuts()
{
    shortcuts()->createAction("moveClientLeft", &moveClientLeft);
    shortcuts()->createAction("moveClientRight", &moveClientRight);
    shortcuts()->createAction("moveClientUp", &moveClientUp);
    shortcuts()->createAction("moveClientDown", &moveClientDown);

    shortcuts()->createAction("moveHSplitRight", &moveHSplitRight);
    shortcuts()->createAction("moveHSplitLeft", &moveHSplitLeft);
    shortcuts()->createAction("moveVSplitDown", &moveVSplitDown);
    shortcuts()->createAction("moveVSplitUp", &moveVSplitUp);

    shortcuts()->createAction("togglePrimaryExpanding", &togglePrimaryExpanding);
    shortcuts()->createAction("toggleSecondaryExpanding", &toggleSecondaryExpanding);
}

void Mode3Panel::tileClient(Client *client, ContainerContainer *root_container)
{
    assert(root_container->activeClientContainer());

    root_container->activeClientContainer()->addChild(client);
}

void Mode3Panel::moveHSplitRight()
{
    moveSplitter(true, 100);
}

void Mode3Panel::moveHSplitLeft()
{
    moveSplitter(true, -100);
}

void Mode3Panel::moveVSplitDown()
{
    moveSplitter(false, 100);
}

void Mode3Panel::moveVSplitUp()
{
    moveSplitter(false, -100);
}

void Mode3Panel::moveClientLeft()
{
    moveClient(LEFT);
}

void Mode3Panel::moveClientRight()
{
    moveClient(RIGHT);
}

void Mode3Panel::moveClientUp()
{
    moveClient(UP);
}

void Mode3Panel::moveClientDown()
{
    moveClient(DOWN);
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
        if (!root->isEmpty()) {
            Container *c = root->child(0);

            c->setFixedWidth(c->fixedWidth() + delta);
        }
    } else if (!horizontal && root->isHorizontal()) {
        if (root->numElements() == 2) {
            if (root->child(1)->isContainerContainer() && root->child(1)->numElements()) {

                ContainerContainer *c = static_cast<ContainerContainer*>(root->child(1));

                printvar(c->child(0)->isFixedSize());
                c->child(0)->setFixedHeight(c->child(0)->fixedHeight() + delta);
            }
        }
    }
}

void Mode3Panel::togglePrimaryExpanding()
{
    ContainerContainer *root = Application::activeWorkspace()->rootContainer();

    if (!root->isEmpty()) {
        Container *c = root->child(0);
        c->enableFixedSize(!c->isFixedSize());
    }
}

void Mode3Panel::toggleSecondaryExpanding()
{
    ContainerContainer *root = Application::activeWorkspace()->rootContainer();

    if ((root->numElements() == 2) && root->child(1)->isContainerContainer()) {
        ContainerContainer *c = static_cast<ContainerContainer*>(root->child(1));
        if (!c->isEmpty())
            c->child(0)->enableFixedSize(!c->child(0)->isFixedSize());
    }
}
