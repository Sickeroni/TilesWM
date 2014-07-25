#ifndef __CLIENT_FRAME_H__
#define __CLIENT_FRAME_H__

#include "child_widget.h"
#include "client_wrapper.h"
#include "application.h"
#include "backend.h"

class ClientFrame final : public ChildWidget, public Client::DragHandler, public Client::PropertyListener
{
public:
    ClientFrame(ClientWrapper *client);
    ~ClientFrame();

    virtual void setRect(const Rect &rect) override;

    bool hasDecoration() { return _has_decoration; }
    void setHasDecoration(bool has_decoration);
    ClientWrapper *client() { return _client; }

private:
    enum Anchor {
        ANCHOR_LEFT,
        ANCHOR_RIGHT,
        ANCHOR_TOP,
        ANCHOR_BOTTOM,
        ANCHOR_TOP_LEFT,
        ANCHOR_TOP_RIGHT,
        ANCHOR_BOTTOM_LEFT,
        ANCHOR_BOTTOM_RIGHT
    };

    virtual void draw(Canvas *canvas) override;

    virtual void handleButtonPress(int x_global, int y_global, int button) override;
    virtual void handleButtonRelease(int button) override;
    virtual void handleMouseMove(int x, int y) override;

    // Client::DragHandler
    virtual void startDrag(int x_global, int y_global, Client::DragMode mode) override;
    virtual void handleFocusChanged() {
        redraw();
    }

    // Client::PropertyListener
    virtual void propertyChanged(Client *client, ClientBackend::Property property) override;

    void finishDrag();
    void cancelDrag();
    void updateFrameGeometry();
    void adjustMouseCoordinates(int x_global, int y_global, int &x_parent, int &y_parent);
    bool snapCoordinate(int &coord, int snap_coord);

    ClientWrapper *_client = 0;
    Client::DragMode _drag_mode = Client::DRAG_NONE;
    int _drag_start_x = 0;
    int _drag_start_y = 0;
    Rect _rect_before_drag_start;
    bool _has_decoration = false;
    bool _snap_to_border = true;
    int _border_snapping_zone = 20;
};

#endif
