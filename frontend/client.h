#ifndef __CLIENT_H__
#define __CLIENT_H__

#include "child_widget.h"
#include "client_backend.h"
#include "client_frontend.h"
#include "widget_backend.h"
#include "widget_frontend.h"
#include "common.h"

#include <string>
#include <list>

class Icon;
class Workspace;

class Client final : public ChildWidget, public ClientFrontend
{
public:
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

    //FIXME rename
    struct DragHandler {
        virtual void startDrag(int x_global, int y_global, DragMode mode) {}
        virtual void handleFocusChanged() = 0;
    };

    Client(ClientBackend *client_backend);
    ~Client();

    virtual void setRect(const Rect &rect) override {
        ChildWidget::setRect(rect);
        _client_backend->widget()->setRect(rect);
    }

    const ClientBackend *backend() { return _client_backend; }
    Workspace *workspace() { return _workspace; }
    void setWorkspace(Workspace *workspace);
    void applySizeHints(Rect &rect);
    void setDragHandler(DragHandler *handler) {
        _drag_handler = handler;
    }

    void requestClose() {
        _client_backend->requestClose();
    }
    void setFocus() {
        _client_backend->setFocus();
    }

    static void limitRect(Rect &rect);

private:
    enum {
        MIN_WIDTH = 100,
        MIN_HEIGHT = 100,
        //FIXME - use screen size
        MAX_WIDTH = 2000,
        MAX_HEIGHT = 2000
    };

    virtual void draw(Canvas *canvas) override {}
    virtual void handleButtonPress(int x_global, int y_global, int button) override;
    virtual void handleFocusChanged(bool has_focus) override;
    virtual void handleMap() override;
    virtual void handlePropertyChanged(ClientBackend::Property property) override;
    virtual void handleConfigureRequest(const Rect &rect) override;
    ClientBackend *_client_backend = 0;
    Workspace *_workspace = 0;
    DragHandler *_drag_handler = 0;
};

#endif // __CLIENT_H__
