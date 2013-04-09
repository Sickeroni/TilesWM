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

Container::Container(Container *parent, int x, int y, int w, int h) :
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



Container *Container::getChildWestOf(Container *child)
{
    if (isHorizontal()) {
        return getPrev(child);
    } else {
        if (!_parent) { // we're the root container
            ContainerContainer *new_root =
                new ContainerContainer(0 ,0, 0, _root->width(), _root->height());
            ClientContainer *new_west =
                new ClientContainer(new_root, 0, 0, _root->width(), _root->height());
            new_root->addContainer(new_west);
            new_root->addContainer(_root);
            _root = new_root;
            return new_west;
        } else {
            return _parent->getChildWestOf(this);
        }
    }
}

#if 0
Container *Container::getWest()
{
    if (!_parent) { // we need to create a ew root container
        ClientContainer *new_west = 
        ContainerContainer *new_root =
            new ContainerContainer(0 ,0, 0, _root->width(), _root->height());
        new_root->addContainer(_root);

    }
}
#endif


// void Container::moveClientRight()
// {
//     Client *current = 0;
// 
//     
// }
// 
// Container *Container::east()
// {
//     if (_orientation == HORIZONTAL)
//         return next();
//     else {
//         return _parent->east();
//     }
// }
