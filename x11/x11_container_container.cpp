#include "x11_container_container.h"

#include "x11_client_container.h"

X11ContainerContainer::X11ContainerContainer(X11ContainerContainer *parent) : ContainerContainer(parent)
{
}

void X11ContainerContainer::setRect(const Rect &rect)
{

}

ClientContainer *X11ContainerContainer::createClientContainer()
{
    X11ClientContainer *client_container = new X11ClientContainer(this);
    return client_container;
}
