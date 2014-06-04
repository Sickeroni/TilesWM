#include "container.h"

#include "workspace.h"
#include "client_container.h"
#include "container_container.h"
#include "application.h"

#include <stdlib.h>


Container::Orientation Container::_root_orientation = HORIZONTAL;



Container::Container(Type type) :
    _parent(0),
    _workspace(0),
    _type(type),
     _fixed_width(0),
    _fixed_height(0),
    _is_fixed_size(false)
{
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
    _rect.set(rect);
}

void Container::rotateOrientation()
{
    if (_root_orientation == VERTICAL)
        _root_orientation = HORIZONTAL;
    else
        _root_orientation = VERTICAL;
}

bool Container::isActive()
{
    if (workspace() && workspace()->isActive() &&
                (Application::self()->activeLayer() == Application::LAYER_TILED)) {
        if (_parent && (_parent->isActive() && (_parent->activeChild() == this)))
            return true;
        else if (_parent)
            return false;
        else
            return true;
    } else
        return false;
}

bool Container::isMinimized()
{
    if (workspace()->maximized()) {
        if (parent()) {
            if (parent()->isActive() && parent()->activeChild() == this)
                return false;
            else
                return true;
        } else
            return false;
    } else
        return false;
}

bool Container::isMaximized()
{
    return workspace()->maximized() && !isMinimized();
}

void Container::makeActive()
{
    workspace()->makeActive();
    if (_parent) {
        _parent->makeActive();
        _parent->setActiveChild(_parent->indexOfChild(this));
    }
    Application::self()->setActiveLayer(Application::LAYER_TILED);
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
    assert(!_parent);
    _workspace = workspace;
}

Workspace *Container::workspace()
{
    if (_parent) {
        assert(!_workspace);
        return _parent->workspace();
    } else
        return _workspace;
}

void Container::setFixedWidth(int width)
{
    _fixed_width = width;
    if (_fixed_width < 0)
        _fixed_width = 0;
	if (parent())
		parent()->handleSizeHintsChanged(this);
}

void Container::setFixedHeight(int height)
{
    _fixed_height = height;
    if (_fixed_height < 0)
        _fixed_height = 0;
	if (parent())
		parent()->handleSizeHintsChanged(this);
}

void Container::enableFixedSize(bool enable)
{
    if (_is_fixed_size != enable) {
        _is_fixed_size = enable;
        if (parent())
            parent()->handleSizeHintsChanged(this);
    }
    redrawAll();
}

Client *Container::activeClient()
{
    if (activeClientContainer())
        return activeClientContainer()->activeClient();
    else
        return 0;
}
