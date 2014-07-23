#ifndef __CLIENT_FRAME_H__
#define __CLIENT_FRAME_H__

#include "child_widget.h"
#include "client_wrapper.h"
#include "application.h"
#include "backend.h"

class ClientFrame final : public ChildWidget
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

    enum DragMode {
        DRAG_NONE,
        DRAG_MOVE,
        DRAG_RESIZE
    };

    enum {
        //FIXME
        MOVE_BUTTON = 1,
        RESIZE_BUTTON = 3
    };

    virtual void draw(Canvas *canvas) override;

    virtual void handleButtonPress(int x_global, int y_global, int button) override;
    virtual void handleButtonRelease(int button) override;
    virtual void handleMouseMove(int x, int y) override;

    void startMove(int x, int y);
    void startResize(Anchor anchor, int x, int y);
    void finishDrag();
    void cancelDrag();

    void updateFrameGeometry();

    ClientWrapper *_client = 0;
    DragMode _drag_mode = DRAG_NONE;
    int _drag_start_x = 0;
    int _drag_start_y = 0;
    Rect _rect_before_drag_start;
    bool _has_decoration = false;
};

#endif
