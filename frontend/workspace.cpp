#include "workspace.h"

#include "window_manager.h"
#include "widget_backend.h"
#include "child_widget.h"
#include "client.h"
#include "client_wrapper.h"
#include "theme.h"
#include "application.h"
#include "mode.h"
#include "backend.h"
#include "icon.h"
#include "common.h"

#include <sstream>

class Workspace::Layout : public ChildWidget {
    std::list<ClientWrapper*> _clients;
    WindowManager *_wm = 0;
    bool _is_active = false;

public:
    Layout(size_t mode) : ChildWidget(OTHER) {
        _wm = Application::self()->mode(mode)->createWindowManager(this);
    }
    virtual ~Layout() {
        assert(_clients.empty());
        delete _wm;
        _wm = 0;
    }

    virtual void draw(Canvas *canvas) override {
        Theme::drawWorkspace(this, 0, canvas);
    }
    virtual void setRect(const Rect &r) override {
        ChildWidget::setRect(r);
        _wm->handleParentWidgetSizeChanged();
    }

    ClientWrapper *activeClient() {
        return _wm->activeClient();
    }

    void makeClientActive(const Client *client) {
        for (auto it = _clients.begin(); it != _clients.end(); it++) {
            ClientWrapper *wrapper = *it;
            if (wrapper->wrappedClient() == client) {
                _wm->makeClientActive(wrapper);
                break;
            }
        }
    }

    WindowManager *wm() { return _wm; }

    void setActive(bool active) {
        _is_active = active;
        if (!active)
            setMapped(false);
        _wm->setActive(active);
        for (ClientWrapper *client : _clients)
            client->setActive(active);
        if (active)
            setMapped(true);
    }

    void addClient(Client *client) {
        ClientWrapper *wrapper = new ClientWrapper(client);
        _clients.push_back(wrapper);
        wrapper->setActive(_is_active);
        _wm->manageClient(wrapper);
    }

    void removeClient(Client *client) {
        for (auto it = _clients.begin(); it != _clients.end(); it++) {
            ClientWrapper *wrapper = *it;
            if (wrapper->wrappedClient() == client) {
                _clients.erase(it);
                _wm->unmanageClient(wrapper);
                delete wrapper;
                return;
            }
        }
    }
};


Workspace::Workspace() : Widget(WORKSPACE),
    _monitor(0),
    _mode(Application::self()->defaultMode())
{
    for (size_t i = 0; i < Application::self()->numModes(); i++)
        _layouts.push_back(new Layout(i));
    assert(!_layouts.empty());

    _active_layout = _layouts[0];
    _active_layout->setActive(true);

    _background = Application::self()->backend()->loadImage("wallpaper.png");
}

Workspace::~Workspace()
{
    assert(_clients.empty());
    assert(!monitor());

    _active_layout->setActive(false);
    _active_layout = 0;
    for (Layout *l : _layouts)
        delete l;
    _layouts.clear();

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
        for (Layout *l : _layouts)
            l->setRect(r);
    }
}

void Workspace::draw(Canvas */*canvas*/)
{
}

void Workspace::layoutContents()
{
    _active_layout->wm()->layout();
}

bool Workspace::makeActive()
{
    if (monitor()) {
        Application::self()->setActiveMonitor(monitor());
        return true;
    } else
        return false;
}

void Workspace::addClient(Client *client)
{
    _clients.push_back(client);

    client->setWorkspace(this);

    for (Layout *l : _layouts)
        l->addClient(client);

    _active_layout->makeClientActive(client);
}

void Workspace::removeClient(Client *client)
{
    for (Layout *l : _layouts)
        l->removeClient(client);

    client->setWorkspace(0);

    _clients.remove(client);
}

ClientWrapper *Workspace::activeClient()
{
    return _active_layout->activeClient();
}

Mode *Workspace::mode()
{
    return Application::self()->mode(_mode);
}

void Workspace::setMode(size_t index)
{
    if (monitor())
        setMapped(false);

    assert(index < _layouts.size());

    const Client *active_client = activeClient() ? activeClient()->wrappedClient() : 0;

    _active_layout->wm()->setHasFocus(false);
    _active_layout->setActive(false);

    _mode = index;

    _active_layout = _layouts[index];

    _active_layout->makeClientActive(active_client);

    _active_layout->setActive(true);
    _active_layout->wm()->setHasFocus(_has_focus);

    if (monitor())
        setMapped(true);

    refreshStatusText();

    Application::self()->focusActiveClient();
}

void Workspace::redrawAll()
{
    _active_layout->wm()->redrawAll();
}

void Workspace::refreshStatusText()
{
    if (_monitor) {
        const std::string &mode_name = Application::self()->mode(modeIndex())->displayName();

        std::stringstream text;
        text<<"Mode: "<<mode_name<<" ("<<(modeIndex() + 1)<<"/"<<Application::self()->numModes()<<")";

        _monitor->setStatusText(text.str());
    }
}

void Workspace::setHasFocus(bool has_focus)
{
    _active_layout->wm()->setHasFocus(has_focus);
    _has_focus = has_focus;
}

void Workspace::makeClientActive(const Client *client)
{
    _active_layout->makeClientActive(client);
}

bool Workspace::handleKeySequence(const AbstractKeySequence *sequence)
{
    return _active_layout->wm()->handleKeySequence(sequence);
}
