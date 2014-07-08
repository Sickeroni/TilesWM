#include "client.h"

#include "widget_backend.h"
#include "application.h"
#include "common.h"

Client::Client(ClientBackend *client_backend, bool is_floating) : ChildWidget(CLIENT),
    _client_backend(client_backend)
{
    printvar(is_floating);
    _backend = _client_backend->widget();
    _backend->setFrontend(this);
    _rect = _client_backend->rect();
//     _is_floating = is_floating;
//     abort();
}

Client::~Client()
{
    _backend->setFrontend(0);
}

void Client::handleGeometryChanged(const Rect &rect)
{
    _rect = rect;
}

void Client::handleFocusChanged(bool has_focus)
{
    //FIXME
    // this should be overidden in a subclass like TiledClient


    if (has_focus)
        Application::makeClientActive(this);

    //container()->handleClientFocusChanged(this);
    //HACK
    if (parent())
        parent()->redraw();
}

void Client::handleSizeHintsChanged()
{
    UNIMPLEMENTED
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
