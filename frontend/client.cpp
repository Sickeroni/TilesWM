#include "client.h"

#include "widget_backend.h"
#include "application.h"
#include "common.h"

Client::Client(ClientBackend *client_backend, bool is_floating) : ChildWidget(CLIENT),
    _client_backend(client_backend)
{
    _backend = _client_backend->widget();
    _rect = _client_backend->rect();
    _is_floating = is_floating;
}

Client::~Client()
{
}

bool Client::hasFocus()
{
    return _client_backend->hasFocus();
}

bool Client::hasDecoration()
{
    return true;
}

const std::string &Client::name()
{
    return _client_backend->name();
}

Icon *Client::icon()
{
    return _client_backend->icon();
}

void Client::setFocus()
{
    _client_backend->setFocus();
}

void Client::requestClose()
{
    _client_backend->requestClose();
}

void Client::handleGeometryChanged(const Rect &rect)
{
    _rect = rect;
}

void Client::handleFocusChanged(bool has_focus)
{
    //FIXME
    // if (has_focus && !isActive())
    //      makeActive()
//     container()->handleClientFocusChanged(this);
}

void Client::handleSizeHintsChanged()
{
//FIXME
    // WARNING frame dimensions must be added !
#if 0
    if (parent()) {
        if (Container *container = parent()->toContainer())
            container->handleSizeHintsChanged(this);
    }
#endif
}

void Client::handleMap()
{
    Application::manageClient(this);
}

void Client::handlePropertyChanged(ClientBackend::Property property)
{
    switch (property) {
        case ClientBackend::PROP_SIZE_HINTS:
            handleSizeHintsChanged();
            break;
        case ClientBackend::PROP_NAME:
        case ClientBackend::PROP_ICON_NAME:
        case ClientBackend::PROP_CLASS:
        case ClientBackend::PROP_ICON:
            redraw();
            //FIXME notify parent
            break;
        default:
            debug<<"unhandled property:"<<property;
    }
}
