#include "x11_container_container.h"

#include "x11_client_container.h"
#include "x11_server_widget.h"

#include <stdlib.h>

X11ContainerContainer::X11ContainerContainer(X11ContainerContainer *parent) :
    ContainerContainer(parent),
    _widget(X11ServerWidget::create(parent ? parent->widget() : 0))
{
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
    ContainerContainer::setRect(rect);
    _widget->setRect(rect);
}

ClientContainer *X11ContainerContainer::createClientContainer()
{
    X11ClientContainer *client_container = new X11ClientContainer(this);
    return client_container;
}
