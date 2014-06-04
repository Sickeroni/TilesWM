#include "workspace.h"

#include "container_container.h"
#include "container_layout.h"
#include "container_util.h"
#include "application.h"
#include "mode.h"
#include "common.h"

Workspace::Workspace() :
    _monitor(0),
    _root_container(Application::self()->createContainerContainer()),
    _maximized(false),
    _mode(Application::self()->defaultMode())
{
    _root_container->setWorkspace(this);

    mode()->activate(this);

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

Mode *Workspace::mode()
{
    return Application::self()->mode(_mode);
}

void Workspace::setMode(size_t index)
{
    assert(index < Application::self()->numModes());

    _mode = index;
    mode()->activate(this);

    Application::focusActiveClient();
}
