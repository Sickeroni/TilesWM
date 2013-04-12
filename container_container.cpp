#include "container_container.h"

#if 1

#include <stdlib.h>


ContainerContainer::ContainerContainer() : Container(CONTAINER),
    _dirty(true)
{
}


ClientContainer *ContainerContainer::activeClientContainer()
{
    //FIXME
    abort();
}


void ContainerContainer::addClient(Client *c)
{
    //FIXME
    abort();
}

void ContainerContainer::layout()
{
    //FIXME
    abort();
}


void ContainerContainer::prependChild(Container *container)
{
    //FIXME
    abort();
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
    //FIXME
    abort();
}


void ContainerContainer::setDirty(bool set)
{
    //FIXME
    abort();
#if 0
    _dirty = set;

    if (_parent) {
        if (_dirty)
            _parent->setDirty(true);
        else
            _parent->updateDirtyStatus();
    }
#endif
}



void ContainerContainer::deleteEmptyChildren()
{
    //FIXME
#if 0
    if (!_dirty)
        return;

    //loop through children
    {
        if (child->isEmpty()) {
            //delete child
        } else if (child->isContainerContainer())
            ContainerContainer *c = static_cast<ContainerContainer*>(child);
            if (c->isDirty())
                c->deleteEmptyChildren();
            if (c->isEmpty()) {
                //delete child
            }
        }

    _dirty = !hasChildren();

    if (_dirty && !_parent) { // TODO - replace this with empty ClientContainer ?

    }
#else
    abort();
#endif
}


#endif
