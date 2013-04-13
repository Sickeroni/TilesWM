#include "container_container.h"

#include "client_container.h"

#if 1

#include <iostream>
#include <stdlib.h>


ContainerContainer::ContainerContainer() : Container(CONTAINER),
    _dirty(true)
{
}


ClientContainer *ContainerContainer::activeClientContainer()
{
    Container *active = _children.first(); //FIXME

    if (active && active->isContainerContainer())
        return static_cast<ContainerContainer*>(active)->activeClientContainer();
    else
        return 0;
}


void ContainerContainer::addClient(Client *c)
{
    activeClientContainer()->addClient(c);
}

void ContainerContainer::layout()
{
    if (!width() || !height())
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

    int i = 0;
    for(Container *c = _children.first(); c; c = c->next()) {

        int x = 0, y = 0;
        if (isHorizontal()) {
            x = i * cell_width;
            y = 0;
        } else {
            x = 0;
            y = i * cell_width;
        }

        localToGlobal(x, y);

        c->setRect(x, y, cell_width, cell_height);

        i++;
   }
}


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


void ContainerContainer::appendChild(Container *container)
{
    if (container->parent())
        abort();
    if (!container->isUnlinked())
        abort();

    _children.append(container);
    container->setParent(this);

    layout();
}

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

#endif
