#include "x11_client_container.h"

#include "x11_container_container.h"

X11ClientContainer::X11ClientContainer(X11ContainerContainer *parent) :
    ClientContainer(parent),
    _widget(0)
{
}
