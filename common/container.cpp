#include "container.h"

#if 1

#include "client_container.h"
#include "container_container.h"

#include <stdlib.h>


Container::Orientation Container::_root_orientation = HORIZONTAL;



Container::Container(Type type) :
    _parent(0),
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


#endif
