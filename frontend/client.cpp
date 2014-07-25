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
    if (_client_backend->minWidth() && rect.w < _client_backend->minWidth())
        rect.w = _client_backend->minWidth();
    if (_client_backend->minHeight() && rect.h < _client_backend->minHeight())
        rect.h = _client_backend->minHeight();
    if (_client_backend->maxWidth() && rect.w > _client_backend->maxWidth())
        rect.w = _client_backend->maxWidth();
    if (_client_backend->maxHeight() && rect.h > _client_backend->maxHeight())
        rect.h = _client_backend->maxHeight();
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

#if 0
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
#endif

void Client::handleMap()
{
//     UNIMPLEMENTED
#if 0
    _client_backend->widget()->reparent(_backend);

    updateFrameGeometry();

    _client_backend->widget()->setMapped(true);

    Application::manageClient(this);
#endif
    Application::manageClient(this);
}

void Client::handlePropertyChanged(ClientBackend::Property property)
{
    UNIMPLEMENTED
#if 0
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
#endif
}

void Client::handleConfigureRequest(const Rect &client_rect)
{
    UNIMPLEMENTED
//     assert(false);
#if 0
    if (isFloating()) {
        // new frame rect based on requested client rect
        Rect frame_rect;
        Theme::calcClientFrameRect(hasDecoration(), maxTextHeight(), client_rect, frame_rect);
//         limitFrameRect(frame_rect);

        frame_rect.setPos(client_rect.x, client_rect.y);
//         frame_rect.setPos(rect().x, rect().y);

        setRect(frame_rect);
    }
#endif
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
