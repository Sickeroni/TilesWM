#include "container.h"

#if 1

#include "client_container.h"
#include "container_container.h"

#include <stdlib.h>


Container *Container::_root = 0;
Container::Orientation Container::_root_orientation = HORIZONTAL;


void Container::startup(int screen_width, int screen_height)
{
    _root = new ClientContainer();
    //FIXME set size
}

void Container::shutdown()
{
    delete _root;
    _root = 0;
}

Container::Container(Type type) :
    _parent(0),
    _x(0),  _y(0), _w(0), _h(0),
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



#endif
