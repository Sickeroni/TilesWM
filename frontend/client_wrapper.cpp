#include "client_wrapper.h"
#include "application.h"
#include "backend.h"

ClientWrapper::ClientWrapper(Client *client, WindowManager *wm) : ChildWidget(OTHER),
    _client(client),
    _client_backend(client->backend()),
    _wm(wm)
{
    _rect = _client_backend->rect();
    _backend = Application::self()->backend()->createWidgetBackend();
    _backend->setRect(_rect);

    handleWindowManagerIsActiveChanged();
}

ClientWrapper::~ClientWrapper()
{
    assert(!_wm->isActive());
    if (_client->parent() == this)
        _client->reparent(0, 0);
}

void ClientWrapper::handleWindowManagerIsActiveChanged()
{
    printvar(_wm->isActive());
    if (_wm->isActive()) {
        _client->reparent(this, _backend);
        _client->setRect(Rect(0, 0, rect().w, rect().h));
        _client->setMapped(true);
    }
}

void ClientWrapper::setRect(const Rect &rect)
{
    printvar(_wm->isActive());
    ChildWidget::setRect(rect);
    if (_wm->isActive())
        _client->setRect(Rect(0, 0, rect.w, rect.h));
}
