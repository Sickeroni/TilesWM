#include "container_base.h"

#include "workspace.h"
#include "client_container.h"
#include "container_container.h"

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
