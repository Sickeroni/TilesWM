#include "client_frame.h"
#include "floating_theme.h"

ClientFrame::ClientFrame(ClientWrapper *client) : ChildWidget(OTHER),
    _client(client)
{
    ChildWidget::setRect(_client->rect());

    _client->reparent(this);
    _client->setRect(Rect(0, 0, rect().w, rect().h));
    _client->setEventHandler(this);
    _client->setPropertyListener(this);
    _client->setMapped(true);

    applySizeHints();
    setRequestedRect();
}

ClientFrame::~ClientFrame()
{
    cancelDrag();
    _client->setPropertyListener(0);
    _client->setEventHandler(0);
    _client->reparent(0);
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
    frame_rect.setPos(rect().x, rect().y);

    setRect(frame_rect);
}

void ClientFrame::setRect(const Rect &rect)
{
    if ((rect.w != this->rect().w ) || (rect.h != this->rect().h)) {
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

    if (button == Client::MOVE_BUTTON)
        handleDragStart(x_global, y_global, Client::DRAG_MOVE);
    else if (button == Client::RESIZE_BUTTON)
        handleDragStart(x_global, y_global, Client::DRAG_RESIZE);
}

void ClientFrame::handleButtonRelease(int /*button*/)
{
    finishDrag();
}

void ClientFrame::handleMouseMove(int x_global, int y_global)
{
    if (_drag_mode != Client::DRAG_NONE) {
        int x_parent, y_parent;
        adjustMouseCoordinates(x_global, y_global, x_parent, y_parent);

        int xdiff = x_parent - _drag_start_x;
        int ydiff = y_parent - _drag_start_y;

        if (_drag_mode == Client::DRAG_MOVE) {
            Rect new_rect = rect();
            new_rect.setPos(_rect_before_drag_start.x + xdiff, _rect_before_drag_start.y + ydiff);

            if (_snap_to_border) {
                Rect parent_rect = parent()->rect();

                int snapped_x = new_rect.x;
                if (!snapCoordinate(snapped_x, 0)) {
                    int snapped_right = new_rect.x + new_rect.w;
                    if (snapCoordinate(snapped_right, parent_rect.w))
                        snapped_x = snapped_right - new_rect.w;
                }

                int snapped_y = new_rect.y;
                if (!snapCoordinate(snapped_y, 0)) {
                    int snapped_bottom = new_rect.y + new_rect.h;
                    if (snapCoordinate(snapped_bottom, parent_rect.h))
                        snapped_y = snapped_bottom - new_rect.h;
                }

                new_rect.setPos(snapped_x, snapped_y);
            }

            setRect(new_rect);
        }
        else if (_drag_mode == Client::DRAG_RESIZE) {

            Rect new_rect = rect();

            new_rect.setSize(_rect_before_drag_start.w + xdiff, _rect_before_drag_start.h + ydiff);

            if (_snap_to_border) {
                Rect parent_rect = parent()->rect();

                int snapped_w = new_rect.w;
                int snapped_h = new_rect.h;

                int snapped_right = rect().x + new_rect.w;
                if (snapCoordinate(snapped_right, parent_rect.w))
                    snapped_w = snapped_right - rect().x;

                int snapped_bottom = rect().y + new_rect.h;
                if (snapCoordinate(snapped_bottom, parent_rect.h))
                    snapped_h = snapped_bottom - rect().y;

                new_rect.setSize(snapped_w, snapped_h);
            }

            Rect client_rect;
            Theme::calcClientClientRect(hasDecoration(), maxTextHeight(), new_rect, client_rect);

            _client->applySizeHints(client_rect);
            Client::limitRect(client_rect);

            Rect frame_rect;
            Theme::calcClientFrameRect(hasDecoration(), maxTextHeight(), client_rect, frame_rect);
            Client::limitRect(frame_rect);

            frame_rect.setPos(rect().x, rect().y);

            setRect(frame_rect);
        }
    }
}

void ClientFrame::handleDragStart(int x_global, int y_global, Client::DragMode mode)
{
    //FIXME what if the pointer is already grabbed ?
    assert(_drag_mode == Client::DRAG_NONE);

    WidgetBackend::CursorType cursor = (mode == Client::DRAG_MOVE) ?
        WidgetBackend::CURSOR_MOVE : WidgetBackend::CURSOR_RESIZE_BOTTOM_RIGHT;

    grabMouse(cursor);

    int x_parent, y_parent;
    adjustMouseCoordinates(x_global, y_global, x_parent, y_parent);

    _drag_mode = mode;
    _drag_start_x = x_parent;
    _drag_start_y = y_parent;
    _rect_before_drag_start = rect();
}

void ClientFrame::finishDrag()
{
    debug;

    assert(_drag_mode != Client::DRAG_NONE);

    _drag_mode = Client::DRAG_NONE;
    _drag_start_x = _drag_start_y = 0;
    _rect_before_drag_start.set(0, 0, 0, 0);

    releaseMouse();
}

void ClientFrame::cancelDrag()
{
    if (_drag_mode != Client::DRAG_NONE)
        finishDrag();
}

void ClientFrame::adjustMouseCoordinates(int x_global, int y_global, int &x_parent, int &y_parent)
{
    x_parent = x_global;
    y_parent = y_global;
    parent()->globalToLocal(x_parent, y_parent);

    //restrict mouse coordinates to parent dimensions
    if (x_parent < 0)
        x_parent = 0;
    if (x_parent >= parent()->width())
        x_parent = parent()->width() -1;
    if (y_parent < 0)
        y_parent = 0;
    if (y_parent >= parent()->height())
        y_parent = parent()->height() -1;
}

bool ClientFrame::snapCoordinate(int &coord, int snap_coord)
{
    bool snapped = true;
    if (coord < snap_coord && coord > (snap_coord - _border_snapping_zone))
        coord = snap_coord;
    else if (coord > snap_coord && coord < (snap_coord + _border_snapping_zone))
        coord = snap_coord;
    else
        snapped = false;
    return snapped;
}

void ClientFrame::propertyChanged(Client */*client*/, ClientBackend::Property property)
{
    if (property == ClientBackend::PROP_SIZE_HINTS)
        applySizeHints();
    else if (property == ClientBackend::PROP_REQUESTED_RECT)
        setRequestedRect();
    else
        redraw();
}

void ClientFrame::setRequestedRect()
{
    Rect req = _client->wrappedClient()->backend()->requestedRect();

    if (req.w && req.h) {
        _client->applySizeHints(req);
        Rect frame_rect;
        Theme::calcClientFrameRect(hasDecoration(), maxTextHeight(), req, frame_rect);
        frame_rect.setPos(rect().x, rect().y);
        setRect(frame_rect);
    }
}

void ClientFrame::applySizeHints()
{
    Rect client_rect = _client->rect();
    _client->applySizeHints(client_rect);
    Rect frame_rect;
    Theme::calcClientFrameRect(hasDecoration(), maxTextHeight(), client_rect, frame_rect);
    frame_rect.setPos(rect().x, rect().y);
    setRect(frame_rect);
}
