#include "workspace.h"

#include "window_manager.h"
#include "widget_backend.h"
#include "child_widget.h"
#include "client.h"
#include "client_wrapper.h"
#include "application.h"
#include "mode.h"
#include "backend.h"
#include "icon.h"
#include "common.h"

#include <sstream>

Workspace::Workspace() : Widget(WORKSPACE),
    _monitor(0),
    _mode(Application::self()->defaultMode())
{
    for (size_t i = 0; i < Application::self()->numModes(); i++) {
        WindowManager *wm = Application::self()->mode(i)->createWindowManager(this);
        _window_managers.push_back(wm);
    }
    assert(!_window_managers.empty());
    _window_manager = _window_managers[0];
    _window_manager->setActive(true);

    _background = Application::self()->backend()->loadImage("wallpaper.png");
}

Workspace::~Workspace()
{
    assert(_clients.empty());
    assert(!monitor());

    _window_manager->setActive(false);
    _window_manager = 0;
    for (WindowManager *wm : _window_managers)
        delete wm;
    _window_managers.clear();

    delete _background;
    _background = 0;
    delete _background_scaled;
    _background_scaled = 0;
}

void Workspace::setRect(const Rect &r)
{
    bool size_changed = (r.w != rect().w || r.h != rect().h);
    Widget::setRect(r);

    if (size_changed) {
        if (_background) {
            delete _background_scaled;
            _background_scaled = _background->scale(r.w, r.h);
        }
        for (WindowManager *wm : _window_managers)
            wm->handleWorkspaceSizeChanged();
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
    child->reparent(this);
}

void Workspace::removeChild(ChildWidget *child)
{
    child->reparent(0);
}

void Workspace::addClient(Client *client)
{
    _clients.push_back(client);

    client->setWorkspace(this);

    for (WindowManager *wm : _window_managers) {
        wm->manageClient(client);
    }

    _window_manager->makeClientActive(client);
}

void Workspace::removeClient(Client *client)
{
    for (WindowManager *wm : _window_managers) {
        wm->unmanageClient(client);
    }

    client->setWorkspace(0);

    _clients.remove(client);
}

ClientWrapper *Workspace::activeClient()
{
    return _window_manager->activeClient();
}

Mode *Workspace::mode()
{
    return Application::self()->mode(_mode);
}

void Workspace::setMode(size_t index)
{
    if (monitor())
        setMapped(false);

    assert(index < _window_managers.size());

    _window_manager->setHasFocus(false);
    _window_manager->setActive(false);

    const Client *active_client = activeClient()->wrappedClient();

    _mode = index;

    _window_manager = _window_managers[index];

    _window_manager->makeClientActive(active_client);

    _window_manager->setActive(true);
    _window_manager->setHasFocus(_has_focus);

    if (monitor())
        setMapped(true);

    refreshStatusText();

    Application::self()->focusActiveClient();
}

void Workspace::redrawAll()
{
    _window_manager->redrawAll();
}

void Workspace::refreshStatusText()
{
    if (_monitor) {
        const std::string &mode_name = Application::self()->mode(modeIndex())->name();

        std::stringstream text;
        text<<"Mode: "<<mode_name<<" ("<<(modeIndex() + 1)<<"/"<<Application::self()->numModes()<<")";

        _monitor->setStatusText(text.str());
    }
}

void Workspace::setHasFocus(bool has_focus)
{
    _window_manager->setHasFocus(has_focus);
    _has_focus = has_focus;
}
