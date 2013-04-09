#include "container.h"

#include "client_container.h"
#include "container_container.h"


Container *Container::_root = 0;

void Container::startup(int screen_width, int screen_height)
{
    _root = new ClientContainer(0, 0, 0, screen_width, screen_height);
}

void Container::shutdown()
{
    delete _root;
    _root = 0;
}

Container::Container(ContainerContainer *parent, int x, int y, int w, int h) :
    _parent(parent), _orientation(HORIZONTAL),
    _x(x),  _y(y), _w(w), _h(h)
{
}

void Container::local_to_global(int &x, int &y)
{
    x += this->x();
    y += this->y();

    if (_parent)
        _parent->local_to_global(x, y);
}
