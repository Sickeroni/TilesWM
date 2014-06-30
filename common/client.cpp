#include "client.h"

#include "widget_backend.h"
#include "client_backend.h"
// #include "container.h"
#include "common.h"

Client::Client(WidgetBackend *backend, bool is_floating) : ChildWidget(CLIENT)
{
    _backend = backend;
    _backend->setFrontend(this);
    _backend->clientBackend()->setEventHandler(this);
    _rect = _backend->clientBackend()->rect();
    _is_floating = is_floating;
}

Client::~Client()
{
    _backend->clientBackend()->setEventHandler(0);
    _backend->setFrontend(0);
}

bool Client::hasFocus()
{
    return _backend->clientBackend()->hasFocus();
}

bool Client::hasDecoration()
{
    return true;
}

const std::string &Client::name()
{
    return _backend->clientBackend()->name();
}

Icon *Client::icon()
{
    return _backend->clientBackend()->icon();
}

void Client::setFocus()
{
    _backend->clientBackend()->setFocus();
}

void Client::requestClose()
{
    _backend->clientBackend()->requestClose();
}

void Client::handleGeometryChanged(const Rect &rect)
{
    _rect = rect;
}

void Client::handleFocusChanged()
{
    //FIXME
//     container()->handleClientFocusChanged(this);
}

#if 0
void Client::handleSizeHintsChanged()
{
    //FIXME
    if (parent()) {
        if (Container *container = parent()->toContainer())
            container->handleSizeHintsChanged(this);
    }
}
#endif

void Client::handleMap()
{
    //FIXME
}

void Client::handlePropertyChanged(ClientBackend::Property property)
{
    //FIXME
}
