#include "workspace.h"

#include "container_container.h"

Workspace::Workspace() :
    _root_container(0),
    _maximized(0)
{
}

void Workspace::setMaximized(bool enable)
{
    _maximized = enable;
    _root_container->handleMaximizedChanged();
    _root_container->layout();
}
