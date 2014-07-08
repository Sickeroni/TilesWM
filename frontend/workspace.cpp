#include "workspace.h"

#include "window_manager.h"
#include "widget_backend.h"
#include "child_widget.h"
#include "application.h"
#include "mode.h"
#include "backend.h"
#include "icon.h"
#include "common.h"

Workspace::Workspace() : Widget(WORKSPACE),
    _monitor(0),
    _mode(Application::self()->defaultMode())
{
    _backend = Application::self()->backend()->createWidgetBackend();
    _backend->setFrontend(this);
    _window_manager = mode()->createWindowManager(this);
    _background = Application::self()->backend()->loadImage("wallpaper.png");
}

Workspace::~Workspace()
{
    _active_floating_child = 0;
    assert(!monitor());
    delete _window_manager;
    _window_manager = 0;
    delete _backend;
    _backend = 0;
    delete _background;
    _background = 0;
    delete _background_scaled;
    _background_scaled = 0;
}

void Workspace::setRect(const Rect &rect)
{
    Widget::setRect(rect);
    if (_background) {
        delete _background_scaled;
        _background_scaled = _background->scale(_rect.w, _rect.h);
    }
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

void Workspace::addChild(ChildWidget *child)
{
    child->reparent(this, _backend);
    child->setMapped(true);
}

void Workspace::removeChild(ChildWidget *child)
{
    if (child == _active_floating_child)
        _active_floating_child = 0;
    child->reparent(0, 0);
}

void Workspace::setActiveFloatingChild(ChildWidget *child)
{
    _active_floating_child = child;
}

Client *Workspace::activeFloatingClient()
{
    if (_active_floating_child)
        return _active_floating_child->toClient();
    else
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

    std::vector<Client*> unmanaged_clients;
    _window_manager->unmanageAllClients(unmanaged_clients);

    delete _window_manager;
    _window_manager = mode()->createWindowManager(this);
//     _window_manager->initShortcuts();

    for (Client *c : unmanaged_clients)
        _window_manager->manageClient(c);

    Application::self()->focusActiveClient();
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
