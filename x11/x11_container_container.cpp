#include "x11_container_container.h"

#include "x11_client_container.h"
#include "x11_server_widget.h"
#include "x11_canvas.h"

#include <stdlib.h>


X11ContainerContainer *X11ContainerContainer::create(Workspace *workspace)
{
    return new X11ContainerContainer(workspace, 0);
}


X11ContainerContainer::X11ContainerContainer(Workspace *workspace, X11ContainerContainer *parent) :
    ContainerContainer(parent),
    _widget(X11ServerWidget::create(parent ? parent->widget() : 0, this, ExposureMask))
{
    setWorkspace(workspace);
    _widget->map();
}

X11ContainerContainer::~X11ContainerContainer()
{
    clear();
    delete _widget;
    _widget = 0;
}

void X11ContainerContainer::setRect(const Rect &rect)
{
    _widget->setRect(rect);
    ContainerContainer::setRect(rect);
}

ClientContainer *X11ContainerContainer::createClientContainer()
{
    X11ClientContainer *client_container = new X11ClientContainer(this);
    return client_container;
}

ContainerContainer *X11ContainerContainer::createContainerContainer()
{
    X11ContainerContainer *container = new X11ContainerContainer(workspace(), this);
    return container;
}

void X11ContainerContainer::redraw()
{
    draw(_widget->canvas());
}

void X11ContainerContainer::reparent(ContainerContainer *p)
{
    ContainerContainer::reparent(p);
    _widget->reparent(static_cast<X11ContainerContainer*>(p)->widget());
}

void X11ContainerContainer::deleteEmptyChildren()
{
//     _widget->unmap();
    ContainerContainer::deleteEmptyChildren();
//     _widget->map();
}
