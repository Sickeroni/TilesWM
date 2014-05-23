#include "actions.h"

#include "client_container.h"
#include "container_container.h"
#include "client.h"
#include "workspace.h"
#include "application.h"
#include "common.h"

namespace Actions
{


Container::Orientation orientationOfDirection(Container::Direction dir)
{
    if (dir == Container::WEST || dir == Container::EAST)
        return Container::HORIZONTAL;
    else
        return Container::VERTICAL;
}

bool isForwardDirection(Container::Direction dir)
{
    return (dir == Container::EAST || dir == Container::SOUTH);
}

int hierarchyDepth(Container *container)
{
    if (container->parent())
        return hierarchyDepth(container->parent()) + 1;
    else
        return 0;
}

ClientContainer *createSibling(Container *container, bool prepend)
{
    ClientContainer *new_sibling = 0;
    ContainerContainer *parent = container->parent();

    if (parent) {
        new_sibling = Application::self()->createClientContainer();
        int insert_pos = parent->indexOfChild(container);
        if (!prepend)
            insert_pos++;
        parent->insertChild(new_sibling, insert_pos);
        parent->setActiveChild(insert_pos);
    }

    return new_sibling;
}

ClientContainer *getSibling(Container *container, bool get_prev, bool create_new_if_not_existing)
{
    ContainerContainer *parent = container->parent();
    if (!parent)
        return 0;

    int index = parent->indexOfChild(container);

    if (get_prev && (index > 0))
        return parent->child(index - 1)->activeClientContainer();
    else if (!get_prev && ((index + 1) < parent->numElements()))
        return parent->child(index + 1)->activeClientContainer();
    else if (create_new_if_not_existing)
        return createSibling(container, get_prev);
    else
        return 0;
}

// return: the newly created sibling
ClientContainer *splitContainer(ClientContainer *container, bool prepend)
{
    static const int max_hierarchy_depth = 1;

    ContainerContainer *parent = container->parent();

    if (hierarchyDepth(parent) >= max_hierarchy_depth)
        return 0;

    ContainerContainer *new_parent = Application::self()->createContainerContainer();

    parent->replaceChild(parent->indexOfChild(container), new_parent); // de-parents container

    ClientContainer *new_sibling = Application::self()->createClientContainer();

    // add this + new child container to new parent
    if (prepend) {
        new_parent->addChild(new_sibling);
        new_parent->addChild(container);
    } else {
        new_parent->addChild(container);
        new_parent->addChild(new_sibling);
    }

    new_parent->setActiveChild(new_parent->indexOfChild(new_sibling));

    return new_sibling;
}

void moveClient(Container::Direction direction)
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

    if (container->workspace()->maximized())
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
        client->setFocus();
    }
}

void moveClientLeft()
{
    //rootContainer()->widget()->unmap(); //HACK
    moveClient(Container::WEST);
//         rootContainer()->deleteEmptyChildren();
    //rootContainer()->widget()->map(); //HACK
//     rootContainer()->setFocus(); //HACK
}

void moveClientRight()
{
    //rootContainer()->widget()->unmap(); //HACK
    moveClient(Container::EAST);
//         rootContainer()->deleteEmptyChildren();
    //rootContainer()->widget()->map(); //HACK
//     rootContainer()->setFocus(); //HACK
}

void moveClientUp()
{
    //rootContainer()->widget()->unmap(); //HACK
    moveClient(Container::NORTH);
//         rootContainer()->deleteEmptyChildren();
    //rootContainer()->widget()->map(); //HACK
//     rootContainer()->setFocus(); //HACK
}

void moveClientDown()
{
    //rootContainer()->widget()->unmap(); //HACK
    moveClient(Container::SOUTH);
//         rootContainer()->deleteEmptyChildren();
    //rootContainer()->widget()->map(); //HACK
//     rootContainer()->setFocus(); //HACK
}

void deleteEmptyContainers()
{
    Application::activeWorkspace()->rootContainer()->deleteEmptyChildren();
}


} // namespace Actions


#if 0
ClientContainer *ClientContainer::getOrCreateSiblingFor(Container *container, bool get_prev)
{
    if (!get_prev && container->next())
        return container->next()->activeClientContainer();
    else if (get_prev && container->prev())
        return container->prev()->activeClientContainer();
    else
        return createSiblingFor(container, get_prev);
}

ClientContainer *ClientContainer::getSibling(bool get_prev)
{
    if (get_prev && prev())
        return prev()->activeClientContainer();
    else if (!get_prev && next())
        return next()->activeClientContainer();
    else
        return 0;
}

void ClientContainer::moveClientToOther(Client *client, Direction dir)
{
    if (workspace()->maximized())
        return;

    if (client->container() != this)
        abort();

    if (!client->isMapped()) //FIXME is this ok ?
        return;

    bool backward = !isForwardDirection(dir);

    ClientContainer *target = 0;

    if (_parent) {
        if (orientationOfDirection(dir) == _parent->orientation()) { // easy case
            if (numMappedClients() > 1) // only create new direct sibling if container doesn't become empty
                target = getOrCreateSiblingFor(this, backward);
            else
                target = getSibling(backward);
        }

        else { // difficult case:
                 // if client container becomes empty -> use use sibling of parent container;
                 // else: 1. replace this with new parent container; 2. add this and new client container to parent created in step 1

            if (numMappedClients() <= 1) // cant't split - use use sibling of parent container
                target = getOrCreateSiblingFor(_parent, backward);
            else { // split this
//                 target = splitContainer(this, backward);
                target = _parent->splitChild(this, backward);

                if (!target) { // split failed - maximum hierarchy depth exceeded ?
                    target = getOrCreateSiblingFor(_parent, backward);
                }
            }
        }
    } else {
        abort();
    }

    if (target) {
        target->addClient(client);
        target->setActiveClient(client);
        target->makeActive();
        client->setFocus();
//         target->setFocus();
//         ContainerContainer *root = 0;
//         for (ContainerContainer *c = target->parent(); c; c = c->parent()) {
//             root = c;
//         }

//         root->setFocus();
//         root->redrawAll();
    }
}

void ClientContainer::focusSibling(Direction where)
{
    ContainerContainer *parent = 0;
    if (orientationOfDirection(where) == _parent->orientation())
        parent = _parent;
    else
        parent = _parent->parent();

    if (parent) {
        if (isForwardDirection(where))
            parent->focusNextChild();
        else
            parent->focusPrevChild();
    }
}

void ClientContainer::createSibling(Direction where)
{
    std::cout<<"ClientContainer::createSibling()\n";
    bool prepend = !isForwardDirection(where);
    if (orientationOfDirection(where) == _parent->orientation()) {
        _parent->addNewClientContainer(prepend);
    } else
        _parent->splitChild(this, prepend);
}

ClientContainer *ClientContainer::createSiblingFor(Container *container, bool prepend_new_sibling)
{
    ClientContainer *new_sibling = 0;

    if (container->parent())
        new_sibling = container->parent()->addNewClientContainer(prepend_new_sibling);

    return new_sibling;
}



ClientContainer *ContainerContainer::splitChild(Container *child, bool prepend_new_sibling)
{
    if (hierarchyDepth() >= _max_hierarchy_depth)
        return 0;

    // create new parent
    ContainerContainer *new_parent = createContainerContainer();

    replaceChild(child, new_parent); // unlinks container

    child->reparent(new_parent);
    new_parent->appendChild(child);

    ClientContainer *new_sibling = new_parent->createClientContainer();

    // add this + new child container to new parent
    if (prepend_new_sibling)
        new_parent->prependChild(new_sibling);
    else
        new_parent->appendChild(new_sibling);

    _active_child = new_parent;
    new_parent->_active_child = new_sibling;

    layout();

    return new_sibling;
}

void ContainerContainer::prependChild(Container *container)
{
//FIXME
//     if (!container->isUnlinked())
//         abort();

    _children.prepend(container);

    if (!_active_child)
        _active_child = container;

    layout();
}

void ContainerContainer::appendChild(Container *container)
{
    printvar(_children.count());

//FIXME
//     if (!container->isUnlinked())
//         abort();

    _children.append(container);

    printvar(_children.count());

    if (!_active_child)
        _active_child = container;

    if (parent())
        parent()->handleSizeHintsChanged(this);

    layout();
}

void ContainerContainer::replaceChild(Container *old_container, Container *new_container)
{
//FIXME
//     if (!new_container->isUnlinked())
//         abort();

    _children.replace(old_container, new_container);

    if (_active_child == old_container)
        _active_child = new_container;

//     updateDirtyStatus();

    layout();
}

ClientContainer *ContainerContainer::addNewClientContainer(bool prepend)
{
    debug;
    ClientContainer *client_container = createClientContainer();
    if (prepend)
        prependChild(client_container);
    else
        appendChild(client_container);

    _active_child = client_container;

    layout();

    return client_container;
}

void ContainerContainer::focusPrevChild()
{
    if (_active_child && _active_child->prev())
        setActiveChild(_active_child->prev());
}

void ContainerContainer::focusNextChild()
{
    if (_active_child && _active_child->next())
        setActiveChild(_active_child->next());
}
#endif
