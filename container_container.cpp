#include "container_container.h"

#if 1

#include <stdlib.h>


ContainerContainer::ContainerContainer() :
    Container(),
    _dirty(true)
{
}


ClientContainer *ContainerContainer::activeClientContainer()
{
    //FIXME
    abort();
}


#if 0

void ContainerContainer::addClient(Client *c)
{
    //FIXME
    abort();
}

void ContainerContainer::addContainer(Container *container)
{
    //TODO remove from previous container
    if (container->parent())
        abort();
    if (!container->isUnlinked())
        abort();


    if (_first_child)
        _first_child->append(container);
    else
        _first_child = container;

    container->setParent(this);

    layout();
}

void ContainerContainer::layout()
{
    //FIXME
    abort();
}
#endif


void ContainerContainer::prependChild(Container *container)
{
    //FIXME
    abort();
}


void ContainerContainer::appendChild(Container *container)
{
    //FIXME
    abort();
}


void ContainerContainer::replaceChild(Container *old_container, Container *new_container)
{
    //FIXME
    abort();
}

#if 0
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
#endif

#if 0
void ContainerContainer::deleteEmptyChildren()
{
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
}


#endif

#endif
