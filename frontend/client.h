#ifndef __CLIENT_H__
#define __CLIENT_H__

#include "child_widget.h"
#include "client_backend.h"
#include "client_frontend.h"
#include "common.h"

#include <string>

class Icon;

class Client final : public ChildWidget, public ClientFrontend
{
public:
    Client(ClientBackend *client_backend);
    ~Client();

    virtual void setRect(const Rect &rect) override;

    bool isFloating() { return _is_floating; }
    bool hasDecoration() { return _has_decoration; }
    void setHasDecoration(bool has_decoration);
    void setIsFloating(bool is_floating);
    const std::string &name() {
        return _client_backend->name();
    }
    const std::string &className() {
        return _client_backend->className();
    }
    const std::string &iconName() {
        return _client_backend->iconName();
    }
    const std::string &title() {
        return _client_backend->title();
    }
    Icon *icon() {
        return _client_backend->icon();
    }
    void requestClose() {
        _client_backend->requestClose();
    }
    void setFocus() {
        _client_backend->setFocus();
    }
    bool hasFocus() {
        return _client_backend->hasFocus();
    }
    int minWidth() {
        return _client_backend->minWidth();
    }
    int minHeight() {
        return _client_backend->minHeight();
    }
    int maxWidth() {
        return _client_backend->maxWidth();
    }
    int maxHeight() {
        return _client_backend->maxHeight();
    }
    void updateFrameGeometry();

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
        MIN_WIDTH = 100,
        MIN_HEIGHT = 100,
        //FIXME - use screen size
        MAX_WIDTH = 2000,
        MAX_HEIGHT = 2000
    };

    enum {
        //FIXME
        MOVE_BUTTON = 1,
        RESIZE_BUTTON = 3
    };

    virtual void handleButtonPress(int x_gloab, int y_global, int button) override;
    virtual void handleButtonRelease(int button) override;
    virtual void handleMouseMove(int x, int y) override;
    virtual void handleFocusChanged(bool has_focus) override;
    virtual void handleMap() override;
    virtual void handlePropertyChanged(ClientBackend::Property property) override;
    virtual void handleConfigureRequest(const Rect &rect) override;

    void handleSizeHintsChanged();
    void startMove(int x, int y);
    void startResize(Anchor anchor, int x, int y);
    void finishDrag();
    void cancelDrag();
    void applySizeHints(Rect &rect);

    static void limitRect(Rect &rect);

    DragMode _drag_mode = DRAG_NONE;
    int _drag_start_x = 0;
    int _drag_start_y = 0;
    Rect _rect_before_drag_start;
    bool _is_floating = true;
    bool _has_decoration = false;
    ClientBackend *_client_backend = 0;
};

#endif // __CLIENT_H__
