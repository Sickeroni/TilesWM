#include "client.h"

#include "widget_backend.h"
#include "backend.h"
#include "application.h"
#include "theme.h"
#include "common.h"

Client::Client(ClientBackend *client_backend) : ChildWidget(CLIENT),
    _client_backend(client_backend)
{
    _client_backend->widget()->setFrontend(this);
    _client_backend->grabMouseButton(MOVE_BUTTON);
    _client_backend->grabMouseButton(RESIZE_BUTTON);

    _rect = _client_backend->rect();

    _backend = Application::self()->backend()->createWidgetBackend();
    _backend->setRect(_rect);
    _backend->setFrontend(this);

}

Client::~Client()
{
    cancelDrag();

    _client_backend->widget()->setFrontend(0);
    _client_backend->widget()->setMapped(false);
    _client_backend->widget()->reparent(0);

    delete _backend;
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

void Client::updateFrameGeometry()
{
    if (_is_floating) {
        Rect client_rect = _client_backend->rect();

        Rect frame_rect;
        Theme::calcClientFrameRect(hasDecoration(), maxTextHeight(), client_rect, frame_rect);
        frame_rect.setPos(_rect.x, _rect.y);

        setRect(frame_rect);
    } else {
        Rect client_rect;
        Theme::calcClientClientRect(hasDecoration(), maxTextHeight(), _rect, client_rect);
        _client_backend->widget()->setRect(client_rect);
    }
}

void Client::setIsFloating(bool is_floating)
{
    _is_floating = is_floating;
    updateFrameGeometry();
}

void Client::setHasDecoration(bool has_decoration)
{
    printvar(has_decoration);
    _has_decoration = has_decoration;
    updateFrameGeometry();
}

void Client::setRect(const Rect &rect)
{
    if ((rect.w != _rect.w ) || (rect.h != _rect.h)) {
        Rect client_rect;
        Theme::calcClientClientRect(hasDecoration(), maxTextHeight(), rect, client_rect);
        limitRect(client_rect);
        _client_backend->widget()->setRect(client_rect);
    }

    ChildWidget::setRect(rect);
}

void Client::handleFocusChanged(bool has_focus)
{
    //FIXME
    // this should be overidden in a subclass like TiledClient


    if (has_focus)
        Application::makeClientActive(this);

    redraw();

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
    _client_backend->widget()->reparent(_backend);

    updateFrameGeometry();

    _client_backend->widget()->setMapped(true);

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
    raise();
    setFocus();

    if (isFloating()) {
        if (button == MOVE_BUTTON)
            startMove(x_global, y_global);
        else if (button == RESIZE_BUTTON) {
            Anchor anchor = ANCHOR_BOTTOM_RIGHT;
            printvar(_client_backend->rect().w);
            printvar(_rect.w);
            startResize(anchor, x_global, y_global);
        }
    }
}

void Client::handleButtonRelease(int button)
{
    finishDrag();
}

void Client::handleMouseMove(int x, int y)
{
    if (_drag_mode != DRAG_NONE) {
        int xdiff = x - _drag_start_x;
        int ydiff = y - _drag_start_y;

        if (_drag_mode == DRAG_MOVE) {
            Rect new_rect = rect();
            new_rect.setPos(_rect_before_drag_start.x + xdiff, _rect_before_drag_start.y + ydiff);
            setRect(new_rect);
        }
        else if (_drag_mode == DRAG_RESIZE) {

            Rect client_rect = _client_backend->rect();

            client_rect.setSize(_rect_before_drag_start.w + xdiff, _rect_before_drag_start.h + ydiff);

            applySizeHints(client_rect);
            limitRect(client_rect);

            Rect frame_rect;
            Theme::calcClientFrameRect(hasDecoration(), maxTextHeight(), client_rect, frame_rect);
            limitRect(frame_rect);

            frame_rect.setPos(_rect.x, _rect.y);

            setRect(frame_rect);
        }
    }
}

void Client::startResize(Anchor anchor, int x, int y)
{
#if 0
    //FIXME what if the pointer is already grabbed ?
    assert(!_dragged);

#endif
    _backend->grabMouse();

    _drag_mode = DRAG_RESIZE;
    _drag_start_x = x;
    _drag_start_y = y;
    _rect_before_drag_start = _client_backend->rect();
}

void Client::startMove(int x, int y)
{
#if 0
    //FIXME what if the pointer is already grabbed ?
    assert(!_dragged);

#endif
    _backend->grabMouse();

    _drag_mode = DRAG_MOVE;
    _drag_start_x = x;
    _drag_start_y = y;
    _rect_before_drag_start = rect();
}

void Client::finishDrag()
{
    debug;

    assert(_drag_mode != DRAG_NONE);

    _drag_mode = DRAG_NONE;
    _drag_start_x = _drag_start_y = 0;
    _rect_before_drag_start.set(0, 0, 0, 0);

    _backend->releaseMouse();
}

void Client::cancelDrag()
{
    if (_drag_mode != DRAG_NONE)
        finishDrag();
}
