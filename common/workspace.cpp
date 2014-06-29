#include "workspace.h"

#include "window_manager.h"
#include "widget_backend.h"
#include "application.h"
#include "mode.h"
#include "common.h"

Workspace::Workspace() : Widget(WORKSPACE),
    _monitor(0),
    _maximized(false),
    _mode(Application::self()->defaultMode())
{
    _backend = Application::self()->createWidgetBackend();
    _backend->setFrontend(this);
    _window_manager = mode()->createWindowManager(this);
    _window_manager->initShortcuts();
}

Workspace::~Workspace()
{
    assert(!monitor());
    delete _window_manager;
    _window_manager = 0;
    delete _backend;
    _backend = 0;
}

void Workspace::setMaximized(bool enable)
{
    _maximized = enable;
    _window_manager->handleMaximizedChanged();
}

void Workspace::layoutContents()
{
    _window_manager->layout();
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

void Workspace::addClient(Client *client)
{
}

void Workspace::removeClient(Client *client)
{
}

Mode *Workspace::mode()
{
    return Application::self()->mode(_mode);
}

void Workspace::setMode(size_t index)
{
    assert(index < Application::self()->numModes());

    _mode = index;

    delete _window_manager;
    _window_manager = mode()->createWindowManager(this);
    _window_manager->initShortcuts();

    Application::focusActiveClient();
}
#if 0
void Workspace::rotateOrientation()
{
    if (_orientation == Container::HORIZONTAL)
        _orientation = Container::VERTICAL;
    else
        _orientation = Container::HORIZONTAL;

   layoutContents();
}

void Workspace::addChild(Container *container)
{
    container->reparent(this, widget());
    container->setMapped(true);
}

void Workspace::removeChild(Container *container)
{
    container->reparent(0, 0);
}
#endif
