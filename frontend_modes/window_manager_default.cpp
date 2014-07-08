#include "window_manager_default.h"

#include "container_layout.h"
#include "container_container.h"
#include "client_container.h"
#include "client.h"
#include "workspace.h"
#include "application.h"
#include "common.h"


using namespace ContainerUtil;

WindowManagerDefault::WindowManagerDefault(Workspace *workspace, Mode *mode) :
    WindowManager(workspace, mode),
    _root_container(new ContainerContainer())
{
    workspace->addChild(_root_container);
    layout();
}

WindowManagerDefault::~WindowManagerDefault()
{
    workspace()->removeChild(_root_container);
    delete _root_container;
    _root_container = 0;
}

void WindowManagerDefault::performAction(int id)
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
            deleteEmptyChildren(_root_container);
            Application::self()->focusActiveClient();
            break;
        case ACTION_TOGGLE_EXPANDING:
            if (ClientContainer *c = activeClientContainer())
                c->enableFixedSize(!c->isFixedSize());
            break;
        case ACTION_TOGGLE_PARENT_EXPANDING:
            if (ClientContainer *c = activeClientContainer())
                c->parentContainer()->enableFixedSize(!c->parentContainer()->isFixedSize());
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

ClientContainer *WindowManagerDefault::activeClientContainer()
{
    return _root_container->activeClientContainer();
}

Client *WindowManagerDefault::activeClient()
{
    return activeClientContainer() ? activeClientContainer()->activeClient() : 0;
}

void WindowManagerDefault::manageClient(Client *client)
{
    debug;
    if (!_root_container->activeClientContainer()) {
        ClientContainer *c = new ClientContainer();
        int index = _root_container->addChild(c);
        _root_container->setActiveChild(index);
    }

    _root_container->activeClientContainer()->addChild(client);
}

void WindowManagerDefault::unmanageClient(Client *client)
{
    assert(client->workspace() == workspace());

    ClientContainer *container = client->parentTo<ClientContainer>();
    container->removeChild(client);
}

void WindowManagerDefault::unmanageAllClients(std::vector<Client*> &unmanaged_clients)
{
    ContainerUtil::emptyContainer(_root_container, unmanaged_clients);
}

void WindowManagerDefault::layout()
{
    if (workspace()->rect().w && workspace()->rect().h) {
//     _root_container->setRect(Rect(0, 0, workspace()->rect().w, workspace()->rect().h));
        //HACK
        _root_container->setRect(Rect(
            0,
            workspace()->maxTextHeight(),
            workspace()->rect().w,
            workspace()->rect().h - workspace()->maxTextHeight()));

        _root_container->getLayout()->layoutContents();
    }
}

void WindowManagerDefault::setMaximizeActiveContainer(bool set)
{
    _maximize_active_container = set;

    if (_maximize_active_container)
        _root_container->setMinimizeMode(ContainerContainer::MINIMIZE_INACTIVE);
    else
        _root_container->setMinimizeMode(ContainerContainer::MINIMIZE_NONE);
    layout();
}

void WindowManagerDefault::moveClient(Direction direction)
{
    ClientContainer *container = activeClientContainer();
    if (!container)
        return;

    assert(container->workspace() == Application::activeWorkspace());
    assert(container->parent());

    Client *client = container->activeClient();
    if (!client)
        return;

    assert(client->parent() == container);

    if (_maximize_active_container) //FIXME
        return;

    bool backward = !isForwardDirection(direction);

    ClientContainer *target = 0;

    if (container->parentContainer()) {
        if (orientationOfDirection(direction) == container->parentContainer()->orientation()) { // easy case
            if (container->numElements() > 1) // only create new direct sibling if container doesn't become empty
                target = getSibling(container, backward, true);
            else
                target = getSibling(container, backward, false);
        }

        else { // difficult case:
                 // if client container becomes empty -> use sibling of parent container;
                 // else: 1. replace this with new parent container; 2. add this and new client container to parent created in step 1

            if (container->numElements() <= 1) // cant't split - use sibling of parent container
                target = getSibling(container->parentContainer(), backward, true);
            else { // split this
                target = splitContainer(container, backward);

                if (!target) { // split failed - maximum hierarchy depth exceeded ?
                    target = getSibling(container->parentContainer(), backward, true);
                }
            }
        }
    }

    if (target) {
        container->removeChild(client);
        int index = target->addChild(client);
        target->setActiveChild(index);
        makeContainerActive(target);
        client->setFocus();
    }
}

void WindowManagerDefault::changeSize(bool horizontal, int delta)
{
    if (Container *c = activeClientContainer()) {
        Container *target = (c->parentContainer()->isHorizontal() == horizontal) ? c : c->parentContainer();
        if (target->isFixedSize()) {
            if (horizontal)
                target->setFixedWidth(target->fixedWidth() + delta);
            else
                target->setFixedHeight(target->fixedHeight() + delta);
        }
    }
}

void WindowManagerDefault::setFixedSizeToMinimum()
{
    if (ClientContainer *container = activeClientContainer()) {
        if (Client *client = container->activeClient()) {
            container->setFixedWidth(client->minWidth());
            container->setFixedHeight(client->minHeight());
            container->enableFixedSize(true);
        }
    }
}

void WindowManagerDefault::makeContainerActive(Container *container)
{
    assert(container->workspace()->windowManager() == this);

    container->workspace()->makeActive();
    if (ContainerContainer *parent =  container->parentTo<ContainerContainer>()) {
        makeContainerActive(parent);
        parent->setActiveChild(parent->indexOfChild(container));
    }
    Application::self()->setActiveLayer(Application::LAYER_TILED);
}

void WindowManagerDefault::makeClientActive(Client *client)
{
    ClientContainer *container = dynamic_cast<ClientContainer*>(client->parent());
    assert(container);

    makeContainerActive(container);
    container->setActiveChild(container->indexOfChild(client));
}

bool WindowManagerDefault::isContainerActive(Container *container)
{
    assert(container->workspace()->windowManager() == this);

    if (container->workspace()->isActive() &&
        (Application::self()->activeLayer() == Application::LAYER_TILED))
    {
        ContainerContainer *parent = container->parentTo<ContainerContainer>();
        if (parent && isContainerActive(parent) && (parent->activeChild() == container))
            return true;
        else if (container->parent())
            return false;
        else
            return true;
    } else
        return false;
}
