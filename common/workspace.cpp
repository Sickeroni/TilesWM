#include "workspace.h"

#include "container_container.h"
#include "container_layout.h"
#include "application.h"
#include "common.h"

Workspace::Workspace() :
    _monitor(0),
    _root_container(Application::self()->createContainerContainer()),
    _maximized(0)
{
    _root_container->setWorkspace(this);
    _root_container->setMapped(true);
}

Workspace::~Workspace()
{
    assert(!monitor());
    delete _root_container;
    _root_container = 0;
}

void Workspace::setMaximized(bool enable)
{
    _maximized = enable;
    _root_container->handleMaximizedChanged();
    _root_container->getLayout()->layoutContents();
}

void Workspace::layoutContents()
{
    _root_container->setRect(_rect);
}

void Workspace::setRect(Rect rect)
{
    _rect = rect;
    layoutContents();
}

bool Workspace::makeActive()
{
    if (monitor()) {
        Application::self()->setActiveMonitor(monitor());
        return true;
    } else
        return false;
}

Client *Workspace::activeClient()
{
    //FIXME
    return 0;
}

void Workspace::globalToLocal(int &x, int &y)
{
    x -= _rect.x;
    y -= _rect.y;
}
