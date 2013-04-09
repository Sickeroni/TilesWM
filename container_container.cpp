#include "container_container.h"

#include <stdlib.h>


ContainerContainer::ContainerContainer(ContainerContainer *parent, int x, int y, int width, int height) :
    Container(parent, x, y, width, height)
{
}

ClientContainer *ContainerContainer::currentClientContainer()
{
    //FIXME
    abort();
}

ContainerList::iterator ContainerContainer::find(Container *container)
{
    for(ContainerList::iterator it = _children.begin();
        it != _children.end(); it++) {
        if (*it == container)
            return it;
    }
    return _children.end();
}

Container *ContainerContainer::prev(Container *from)
{
    ContainerList::iterator it = find(from);
    if (it == _children.end()) // programming error
        abort();
    if (it == _children.begin())
        return 0;
    it--;
    return *it;
}

Container *ContainerContainer::next(Container *from)
{
    ContainerList::iterator it = find(from);
    if (it == _children.end()) // programming error
        abort();
    it++;
    if (it == _children.end())
        return 0;
    else
        return *it;
}

void ContainerContainer::addContainer(Container *container)
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
            c = prev(which);
        else
            c = next(which);

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
