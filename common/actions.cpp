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

