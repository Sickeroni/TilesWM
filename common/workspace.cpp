#include "workspace.h"

#include "container_layout.h"
#include "container_util.h"
#include "window_manager.h"
#include "application.h"
#include "mode.h"
#include "common.h"

Workspace::Workspace() :
    _monitor(0),
    _maximized(false),
    _mode(Application::self()->defaultMode())
{
    _window_manager = mode()->createWindowManager(this);
    _window_manager->initShortcuts();
}

Workspace::~Workspace()
{
    assert(!monitor());
    delete _window_manager;
    _window_manager = 0;
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

void Workspace::rotateOrientation()
{
    if (_orientation == Container::HORIZONTAL)
        _orientation = Container::VERTICAL;
    else
        _orientation = Container::HORIZONTAL;

   layoutContents();
}
