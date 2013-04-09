#include "container_container.h"

#include <stdlib.h>


ContainerContainer::ContainerContainer(ContainerContainer *parent, int x, int y, int width, int height) :
    Container(parent, x, y, width, height),
    _first_child(0)
{
}

ClientContainer *ContainerContainer::currentClientContainer()
{
    //FIXME
    abort();
}

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

ClientContainer *ContainerContainer::findSilblingOf(Container *which, Direction dir)
{
    if (orientationOfDirection(dir) != orientation()) {
        if (_parent)
            return _parent->findSilblingOf(this, dir);
        else
            return 0;
    } else {
        Container *c = 0;
        if (dir == NORTH || dir == WEST)
            c = which->prev();
        else
            c = which->next();

        return c ? c->currentClientContainer() : 0;
    }
}

ClientContainer *ContainerContainer::getOrCreateSilblingOf(Container *which, Direction dir)
{
    if (ClientContainer *c = findSilblingOf(which, dir))
        return c;
    else {
        //FIXME
        abort();
    }
}
