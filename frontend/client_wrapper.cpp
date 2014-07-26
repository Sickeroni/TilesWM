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
    setPropertyListener(0);
    if (_client->parent() == this) {
        _client->setEventHandler(0);
        _client->reparent(0, 0);
    }
    delete _backend;
    _backend = 0;
}

void ClientWrapper::handleWindowManagerIsActiveChanged()
{
    if (_wm->isActive()) {
        _client->reparent(this, _backend);
        _client->setRect(Rect(0, 0, rect().w, rect().h));
        _client->setEventHandler(_event_handler);
        _client->setMapped(true);
    }
}

void ClientWrapper::setRect(const Rect &rect)
{
    ChildWidget::setRect(rect);
    if (_wm->isActive())
        _client->setRect(Rect(0, 0, rect.w, rect.h));
}

void ClientWrapper::setEventHandler(Client::EventHandler *handler)
{
    _event_handler = handler;
    if (_wm->isActive())
        _client->setEventHandler(handler);
}

void ClientWrapper::setPropertyListener(Client::PropertyListener *listener)
{
    if (_property_listener)
        _client->removePropertyListener(_property_listener);
    if (listener)
        _client->addPropertyListener(listener);
    _property_listener = listener;
}
