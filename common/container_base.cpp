#include "container_base.h"

#include "workspace.h"
#include "client_container.h"
#include "container_container.h"
#include "container_widget.h"
#include "application.h"

ContainerBase::ContainerBase(Type type) : _type(type),
    _widget(Application::self()->createContainerWidget(this))
{
}

ContainerBase::~ContainerBase()
{
    delete _widget;
    _widget = 0;
}

ClientContainer *ContainerBase::toClientContainer()
{
    if (isClientContainer())
        return static_cast<ClientContainer*>(this);
    else
        return 0;
}

ContainerContainer *ContainerBase::toContainerContainer()
{
    if (isContainerContainer())
        return static_cast<ContainerContainer*>(this);
    else
        return 0;
}

Workspace *ContainerBase::toWorkspace()
{
    if (isWorkspace())
        return static_cast<Workspace*>(this);
    else
        return 0;
}

void ContainerBase::setMapped(bool mapped)
{
    _widget->setMapped(mapped);
}

void ContainerBase::redraw()
{
    _widget->redraw();
}

void ContainerBase::setRect(const Rect &rect)
{
    _widget->setRect(rect);
    _rect.set(rect);
}

int ContainerBase::maxTextHeight() const
{
    return _widget->maxTextHeight();
}