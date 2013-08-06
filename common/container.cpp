#include "container.h"

#if 1

#include "workspace.h"
#include "client_container.h"
#include "container_container.h"

#include <stdlib.h>


Container::Orientation Container::_root_orientation = HORIZONTAL;



Container::Container(Type type, ContainerContainer *parent) :
    _parent(parent),
    _type(type),
    _workspace(0),
//     _available_space_portion(0)
    _fixed_width(0),
    _fixed_height(0)
{
    if (parent)
        _workspace = parent->workspace();
}

void Container::localToGlobal(int &x, int &y)
{
    x += _rect.x;
    y += _rect.y;

    if (_parent)
        _parent->localToGlobal(x, y);
}

void Container::globalToLocal(int &x, int &y)
{
    if (_parent)
        _parent->globalToLocal(x, y);
    x -= _rect.x;
    y -= _rect.y;
}

Container::Orientation Container::orientation()
{
    if (!_parent)
        return _root_orientation;
    else if (_parent->isMinimized())
        return _parent->orientation();
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
    if (_parent && (_parent->hasFocus() && (_parent->activeChild() == this)))
        return true;
    else if (_parent) {
        return false;
    } else
        return true;
}

bool Container::isMinimized()
{
    if (workspace()->maximized()) {
        if (parent()) {
            if (parent()->hasFocus() && parent()->activeChild() == this)
                return false;
            else
                return true;
        } else
            return false;
    } else
        return false;
}

void Container::makeActive()
{
    if (_parent) {
        _parent->makeActive();
        _parent->setActiveChild(this);
    }
}

bool Container::isAncestorOf(Container *container)
{
    for (Container *parent = container->parent(); parent; parent = parent->parent()) {
        if (this == parent)
            return true;
    }
    return false;
}

void Container::setWorkspace(Workspace *workspace)
{
    _workspace = workspace;
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


// int Container::size()
// {
//     int size = 0;
//     if (parent())
//         size = parent()->isHorizontal() ? width() : height();
//     return size;
// }
// 
// int Container::minSize()
// {
//     int min_size = 0;
//     if (parent())
//         min_size = parent()->isHorizontal() ? minimumWidth() : minimumHeight();
// 
//     if (_fixed_size > min_size)
//         min_size = _fixed_size;
// 
//     return min_size;
// }
// 
// int Container::maxSize()
// {
//     
// }


int Container::minWidth()
{
    int ret = minWidthInt();
    if (_fixed_width && _fixed_width > ret)
        ret = _fixed_width;
    return ret;
}

int Container::maxWidth()
{
    int ret = maxWidthInt();
    if (_fixed_width && _fixed_width < ret)
        ret = _fixed_width;
    return ret;
}

int Container::minHeight()
{
    int ret = minHeightInt();
    if (_fixed_height && _fixed_height > ret)
        ret = _fixed_height;
    return ret;
}

int Container::maxHeight()
{
    int ret = maxHeightInt();
    if (_fixed_height && _fixed_height < ret)
        ret = _fixed_height;
    return ret;
}

void Container::setFixedWidth(int width)
{
    _fixed_width = width;
    parent()->handleSizeHintsChanged(this);
}

void Container::setFixedHeight(int height)
{
    _fixed_height = height;
    parent()->handleSizeHintsChanged(this);
}

// void Container::setCustomSizeActive(bool active)
// {
// //     if (!active)
//         _custom_size = 0;
// //     else
// //         _custom_size = size();
//     if (parent())
//         parent()->layout();
// 
// }
// 
// void Container::incCustomSize()
// {
//     if (!_custom_size)
//         setCustomSizeActive(true);
//     _custom_size += 100;
//     if (parent())
//         parent()->layout();
// }
// 
// void Container::decCustomSize()
// {
//     if (!_custom_size)
//         setCustomSizeActive(true);
//     _custom_size -= 100;
//     if (_custom_size < 0)
//         _custom_size = 0;
//     if (parent())
//         parent()->layout();
// }


// void Container::incAvailableSpacePortion(int pixels)
// {
//     parent()->incAvailableSpacePortion(this, pixels);
// }
// 
// void Container::decAvailableSpacePortion(int pixels)
// {
//     parent()->decAvailableSpacePortion(this, pixels);
// }


#endif
