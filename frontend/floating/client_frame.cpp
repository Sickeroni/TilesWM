#include "client_frame.h"
#include "frontend_theme.h"
#include "theme.h"

ClientFrame::ClientFrame(ClientWrapper *client) : ChildWidget(OTHER),
    _client(client)
{
    _rect = client->rect();
    _backend = Application::self()->backend()->createWidgetBackend();
    _backend->setRect(_rect);
    _backend->setFrontend(this);

    _client->grabMouseButton(MOVE_BUTTON);
    _client->grabMouseButton(RESIZE_BUTTON);

    _client->reparent(this, _backend);
    _client->setRect(Rect(0, 0, _rect.w, _rect.h));
    _client->setMapped(true);
}

ClientFrame::~ClientFrame()
{
    cancelDrag();
    delete _backend;
    _backend = 0;
}

void ClientFrame::draw(Canvas *canvas)
{
    if (hasDecoration())
        Theme::drawClientFrame(_client, canvas, rect());
}

void ClientFrame::setHasDecoration(bool has_decoration)
{
    printvar(has_decoration);
    _has_decoration = has_decoration;
    updateFrameGeometry();
}

void ClientFrame::updateFrameGeometry()
{
    Rect client_rect = _client->rect();

    Rect frame_rect;
    Theme::calcClientFrameRect(hasDecoration(), maxTextHeight(), client_rect, frame_rect);
    frame_rect.setPos(_rect.x, _rect.y);

    setRect(frame_rect);
}

void ClientFrame::setRect(const Rect &rect)
{
    if ((rect.w != _rect.w ) || (rect.h != _rect.h)) {
        Rect client_rect;
        Theme::calcClientClientRect(hasDecoration(), maxTextHeight(), rect, client_rect);
        Client::limitRect(client_rect);
        _client->setRect(client_rect);
    }

    ChildWidget::setRect(rect);
}

void ClientFrame::handleButtonPress(int x_global, int y_global, int button)
{
    raise();
    _client->setFocus();

    if (button == MOVE_BUTTON)
        startMove(x_global, y_global);
    else if (button == RESIZE_BUTTON) {
        Anchor anchor = ANCHOR_BOTTOM_RIGHT;
        printvar(_client->rect().w);
        printvar(_rect.w);
        startResize(anchor, x_global, y_global);
    }
}

void ClientFrame::handleButtonRelease(int button)
{
    finishDrag();
}

void ClientFrame::handleMouseMove(int x, int y)
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

            Rect client_rect = _client->rect();

            client_rect.setSize(_rect_before_drag_start.w + xdiff, _rect_before_drag_start.h + ydiff);

            _client->applySizeHints(client_rect);
            Client::limitRect(client_rect);

            Rect frame_rect;
            Theme::calcClientFrameRect(hasDecoration(), maxTextHeight(), client_rect, frame_rect);
            Client::limitRect(frame_rect);

            frame_rect.setPos(_rect.x, _rect.y);

            setRect(frame_rect);
        }
    }
}

void ClientFrame::startResize(Anchor anchor, int x, int y)
{

    //FIXME what if the pointer is already grabbed ?
    assert(_drag_mode == DRAG_NONE);

    _backend->grabMouse();

    _drag_mode = DRAG_RESIZE;
    _drag_start_x = x;
    _drag_start_y = y;
    _rect_before_drag_start = _client->rect();
}

void ClientFrame::startMove(int x, int y)
{
    //FIXME what if the pointer is already grabbed ?
    assert(_drag_mode == DRAG_NONE);

    _backend->grabMouse();

    _drag_mode = DRAG_MOVE;
    _drag_start_x = x;
    _drag_start_y = y;
    _rect_before_drag_start = rect();
}

void ClientFrame::finishDrag()
{
    debug;

    assert(_drag_mode != DRAG_NONE);

    _drag_mode = DRAG_NONE;
    _drag_start_x = _drag_start_y = 0;
    _rect_before_drag_start.set(0, 0, 0, 0);

    _backend->releaseMouse();
}

void ClientFrame::cancelDrag()
{
    if (_drag_mode != DRAG_NONE)
        finishDrag();
}
