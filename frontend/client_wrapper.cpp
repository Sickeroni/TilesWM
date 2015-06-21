#include "client_wrapper.h"
#include "application.h"
#include "backend.h"

ClientWrapper::ClientWrapper(Client *client) : ChildWidget(OTHER),
    _client(client),
    _client_backend(client->backend())
{
    ChildWidget::setRect(_client->rect());
}

ClientWrapper::~ClientWrapper()
{
    setPropertyListener(0);
    if (_client->parent() == this) {
        _client->setEventHandler(0);
        _client->reparent(0);
    }
}

void ClientWrapper::setActive(bool active)
{
    _is_active = active;
    if (_is_active) {
        _client->reparent(this);
        _client->setRect(Rect(0, 0, rect().w, rect().h));
        _client->setEventHandler(_event_handler);
        _client->setMapped(true);
    }
}

void ClientWrapper::setRect(const Rect &rect)
{
    ChildWidget::setRect(rect);
    if (_is_active)
        _client->setRect(Rect(0, 0, rect.w, rect.h));
}

void ClientWrapper::setEventHandler(Client::EventHandler *handler)
{
    _event_handler = handler;
    if (_is_active)
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
