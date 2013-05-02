#include "container.h"

#if 1

#include "client_container.h"
#include "container_container.h"

#include <stdlib.h>


Container::Orientation Container::_root_orientation = VERTICAL;



Container::Container(Type type, ContainerContainer *parent) :
    _parent(parent),
    _type(type)
{
}

void Container::localToGlobal(int &x, int &y)
{
    x += this->x();
    y += this->y();

    if (_parent)
        _parent->localToGlobal(x, y);
}

Container::Orientation Container::orientation()
{
    if (!_parent)
        return _root_orientation;
    else
        return _parent->isHorizontal() ? VERTICAL : HORIZONTAL;
}

void Container::setRect(const Rect &rect)
{
    _rect.set(rect.x, rect.y, rect.w, rect.h);
}

void Container::rotateOrientation()
{
    if (_root_orientation == VERTICAL)
        _root_orientation = HORIZONTAL;
    else
        _root_orientation = VERTICAL;
}

bool Container::hasFocus()
{
    if (_parent && _parent->hasFocus() && (_parent->activeChild() == this))
        return true;
    else if (!_parent)
        return true;
    else
        return false;
}


void Container::makeActive()
{
    if (_parent) {
        _parent->makeActive();
        _parent->setActiveChild(this);
    }
}

// ContainerContainer *Container::root()
// {
//     ContainerContainer *root = this;
//     for (Container *c = this; c; c = c->parent())
//         root = c;
// 
//     
//     return root;
// }

#endif
