#include "client.h"

#include "widget_backend.h"
#include "backend.h"
#include "application.h"
#include "theme.h"
#include "common.h"

Client::Client(ClientBackend *client_backend) : ChildWidget(OTHER),
    _client_backend(client_backend)
{
    _backend = _client_backend->widget();
    _backend->setFrontend(this);
    _rect = _client_backend->rect();

    _client_backend->grabMouseButton(MOVE_BUTTON);
    _client_backend->grabMouseButton(RESIZE_BUTTON);
}

Client::~Client()
{
    assert(!_workspace);
    _backend->setFrontend(0);
    _backend->setMapped(false);
    _backend->reparent(0);
}

void Client::setWorkspace(Workspace *workspace)
{
    assert(!_workspace || !workspace);
    _workspace = workspace;
}

void Client::applySizeHints(Rect &rect)
{
    const SizeHints hints = _client_backend->sizeHints();

    if (hints.min_width && rect.w < hints.min_width)
        rect.w = hints.min_width;
    if (hints.min_height && rect.h < hints.min_height)
        rect.h = hints.min_height;
    if (hints.max_width && rect.w > hints.max_width)
        rect.w = hints.max_width;
    if (hints.max_height && rect.h > hints.max_height)
        rect.h = hints.max_height;
}

void Client::limitRect(Rect &rect)
{
    if (rect.w < MIN_WIDTH)
        rect.w = MIN_WIDTH;
    if (rect.h < MIN_HEIGHT)
        rect.h = MIN_HEIGHT;
    if (rect.w > MAX_WIDTH)
        rect.w = MAX_WIDTH;
    if (rect.h > MAX_HEIGHT)
        rect.h = MAX_HEIGHT;
}

void Client::handleFocusChanged(bool has_focus)
{
    if (_drag_handler)
        _drag_handler->handleFocusChanged();
#if 0

    //FIXME
    // this should be overidden in a subclass like TiledClient


    if (has_focus)
        Application::makeClientActive(this);

    redraw();

    //container()->handleClientFocusChanged(this);
    //HACK
    if (parent())
        parent()->redraw();
#endif
}

void Client::handleMap()
{
    Application::manageClient(this);
}

void Client::handlePropertyChanged(ClientBackend::Property property)
{
    for (PropertyListener *listener : _property_listeners)
        listener->propertyChanged(this, property);
}

void Client::handleButtonPress(int x_global, int y_global, int button)
{
    setFocus();

    if (_drag_handler) {
        if (button == MOVE_BUTTON)
            _drag_handler->startDrag(x_global, y_global, DRAG_MOVE);
        else if (button == RESIZE_BUTTON)
            _drag_handler->startDrag(x_global, y_global, DRAG_RESIZE);
    }
}

void Client::addPropertyListener(PropertyListener *listener)
{
    _property_listeners.push_back(listener);
}

void Client::removePropertyListener(PropertyListener *listener)
{
    _property_listeners.remove(listener);
}
