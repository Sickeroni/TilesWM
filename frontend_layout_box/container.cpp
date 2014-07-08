#include "container.h"

#include "client_container.h"
#include "container_container.h"
#include "application.h"
#include "backend.h"
#include "widget_backend.h"
#include "mouse_handler.h"
#include "common.h"

#include <stdlib.h>


Container::Container(Type type) : ChildWidget(OTHER),
    _type(type)
{
    _backend = Application::self()->backend()->createWidgetBackend();
    _backend->setFrontend(this);
}

Container::~Container()
{
    delete _backend;
    _backend = 0;
}

// Container::Orientation Container::orientation()
// {
//     UNIMPLEMENTED
//     assert(0);
// //     if (!_parent)
// //         return HORIZONTAL;
// //     else if (_parent->isWorkspace())
// //         return _parent->toWorkspace()->orientation();
// //     else if (parent()->isMinimized())
// //         return parent()->orientation();
// //     else
// //         return parent()->isHorizontal() ? VERTICAL : HORIZONTAL;
// }

bool Container::isAncestorOf(Container *container) const
{
    for (Container *parent = container->parentContainer(); parent; parent = parent->parentContainer()) {
        if (this == parent)
            return true;
    }
    return false;
}

void Container::setFixedWidth(int width)
{
    _fixed_width = width;
    if (_fixed_width < 0)
        _fixed_width = 0;
    if (parentContainer())
        parentContainer()->handleSizeHintsChanged(this);
}

void Container::setFixedHeight(int height)
{
    _fixed_height = height;
    if (_fixed_height < 0)
        _fixed_height = 0;
    if (parentContainer())
        parentContainer()->handleSizeHintsChanged(this);
}

void Container::enableFixedSize(bool enable)
{
    if (_is_fixed_size != enable) {
        _is_fixed_size = enable;
        if (parentContainer())
            parentContainer()->handleSizeHintsChanged(this);
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

ContainerContainer *Container::parentContainer()
{
    return dynamic_cast<ContainerContainer*>(parent());
}

ClientContainer *Container::toClientContainer()
{
    if (isClientContainer())
        return static_cast<ClientContainer*>(this);
    else
        return 0;
}

ContainerContainer *Container::toContainerContainer()
{
    if (isContainerContainer())
        return static_cast<ContainerContainer*>(this);
    else
        return 0;
}

void Container::handleClick(int x, int y)

{
    MouseHandler::handleContainerClick(this, x, y);
}
