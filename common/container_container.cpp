#include "container_container.h"

#include "client_container.h"

#if 1

#include <iostream>
#include <stdlib.h>


ContainerContainer::ContainerContainer(ContainerContainer *parent) : Container(CONTAINER, parent),
    _dirty(true)
{
}

ContainerContainer::~ContainerContainer()
{
    clear();
}


void ContainerContainer::clear()
{
    for (Container *c = _children.first(); c; ) {
        Container *delete_this= c;
        c = c->next();
        deleteChild(delete_this);
    }
}

Container *ContainerContainer::activeChild()
{
    //FIXME HACK
    return _children.first();
}

ClientContainer *ContainerContainer::activeClientContainer()
{
    if (Container *active = activeChild()) {
        return active->activeClientContainer();
    } else
        return 0;
}


void ContainerContainer::addClient(Client *c)
{
    if(activeChild())
        activeChild()->addClient(c);
    else {
        ClientContainer *client_container = createClientContainer();
        appendChild(client_container);
        client_container->addClient(c);
    }
}

void ContainerContainer::layout()
{
    if (!width() || !height() || !_children.count())
        return;

    int cell_width = 0, cell_height = 0;

    if (isHorizontal()) {
        cell_width = width() / _children.count();
        cell_height = height();
    } else {
        cell_width = width();
        cell_height = height() / _children.count();
    }

    std::cout<<"cell_width: "<<cell_width<<"\n";
    std::cout<<"cell_height: "<<cell_height<<"\n";
    std::cout<<"=================================\n";

    Rect new_rect;
    new_rect.setSize(cell_width, cell_height);

    int i = 0;
    for(Container *c = _children.first(); c; c = c->next()) {
        if (isHorizontal()) {
            new_rect.x = i * cell_width;
            new_rect.y = 0;
        } else {
            new_rect.x = 0;
            new_rect.y = i * cell_width;
        }

//         localToGlobal(x, y);

        c->setRect(new_rect);
        c->layout();

        i++;
   }
}

#if 0
void ContainerContainer::prependChild(Container *container)
{
    if (container->parent())
        abort();
    if (!container->isUnlinked())
        abort();

    _children.prepend(container);
    container->setParent(this);

    layout();
}
#endif

void ContainerContainer::appendChild(Container *container)
{
    if (!container->isUnlinked())
        abort();

    _children.append(container);

    layout();
}


#if 0

void ContainerContainer::replaceChild(Container *old_container, Container *new_container)
{
    if (new_container->parent())
        abort();
    if (!new_container->isUnlinked())
        abort();

    _children.replace(old_container, new_container);

    //TODO if (_active_container == old_container)
    //          _active_container = new_container;

    updateDirtyStatus();

    layout();
}

#endif


void ContainerContainer::setDirty(bool set)
{
    _dirty = set;

    if (_parent) {
        if (_dirty)
            _parent->setDirty(true);
        else
            _parent->updateDirtyStatus();
    }
}

void ContainerContainer::updateDirtyStatus()
{
    //FIXME a bit ugly

    bool dirty = false;
    for(Container *c = _children.first(); c; c = c->next()) {
        if (c->isEmpty()) {
            dirty = true;
            break;
        } else if (c->isContainerContainer()) {
            if (static_cast<ContainerContainer*>(c)->_dirty) {
                dirty = true;
                break;
            }
        }
    }
    setDirty(dirty);
}

void ContainerContainer::deleteEmptyChildren()
{
    if (!_dirty)
        return;

    for (Container *child = _children.first(); child; ) {
        Container *delete_this = 0;

        if (child->isEmpty())
            delete_this = child;
        else if (child->isContainerContainer()) {
            ContainerContainer *c = static_cast<ContainerContainer*>(child);
            if (c->_dirty)
                c->deleteEmptyChildren();
            if (c->isEmpty()) {
                delete_this = child;
            }
        }

        child = child->next();

        deleteChild(delete_this);
    }

    _dirty = isEmpty();

    if (_dirty && !_parent) { // TODO - replace this with empty ClientContainer ?

    }
}


void ContainerContainer::deleteChild(Container *child)
{
    //TODO if (_active_child)

    _children.remove(child);

    delete child;
}

#if 0
ClientContainer *ContainerContainer::splitChild(Container *child, bool prepend_new_silbling)
{
    // create new parent
    ContainerContainer *new_parent = createContainerContainer();

    replaceChild(child, new_parent); // unlinks container

    child->reparent(new_parent);
    new_parent->appendChild(new_silbling);

    ClientContainer *new_silbling = new_parent->createClientContainer();

    // add this + new child container to new parent
    if (prepend_new_silbling)
        new_parent->preendChild(new_silbling);
    else
        new_parent->appendChild(new_silbling);

    return new_silbling;
}
#endif


#endif
