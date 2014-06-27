#include "container.h"

#include "workspace.h"
#include "client_container.h"
#include "container_container.h"
#include "container_widget.h"
#include "application.h"

#include <stdlib.h>


Container::Container(Type type) : ContainerBase(type),
    _parent(0),
    _fixed_width(0),
    _fixed_height(0),
    _is_fixed_size(false)
{
}

Container::~Container()
{
}

Container::Orientation Container::orientation()
{
    if (!_parent)
        return HORIZONTAL;
    else if (_parent->isWorkspace())
        return _parent->toWorkspace()->orientation();
    else if (parent()->isMinimized())
        return parent()->orientation();
    else
        return parent()->isHorizontal() ? VERTICAL : HORIZONTAL;
}

bool Container::isActive()
{
    if (workspace() && workspace()->isActive() &&
        (Application::self()->activeLayer() == Application::LAYER_TILED))
    {
        if (parent() && (parent()->isActive() && (parent()->activeChild() == this)))
            return true;
        else if (parent())
            return false;
        else
            return true;
    } else
        return false;
}

void Container::makeActive()
{
    workspace()->makeActive();
    if (parent()) {
        parent()->makeActive();
        parent()->setActiveChild(parent()->indexOfChild(this));
    }
    Application::self()->setActiveLayer(Application::LAYER_TILED);
}

bool Container::isAncestorOf(Container *container) const
{
    for (Container *parent = container->parent(); parent; parent = parent->parent()) {
        if (this == parent)
            return true;
    }
    return false;
}

Workspace *Container::workspace()
{
    if (_parent) {
        if (_parent->isWorkspace())
            return _parent->toWorkspace();
        else
            return _parent->toContainerContainer()->workspace();
    } else
        return 0;
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

void Container::reparent(ContainerBase *parent, ContainerWidget *parent_widget)
{
    assert(!hasParent() || !parent);

    setMapped(false);

    widget()->reparent(parent_widget);
    _parent = parent;
}
