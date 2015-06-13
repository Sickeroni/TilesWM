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

    struct EventHandler {
        //FIXME - pass client object
        virtual void handleDragStart(int /*x_global*/, int /*y_global*/, DragMode /*mode*/) {}
        virtual void handleFocusChanged() = 0;
    };

    struct PropertyListener {
        virtual void propertyChanged(Client *client, ClientBackend::Property property) = 0;
    };

    Client(ClientBackend *client_backend);
    ~Client();

    const ClientBackend *backend() const { return _client_backend; }
    Workspace *workspace() { return _workspace; }
    void setWorkspace(Workspace *workspace);
    void applySizeHints(Rect &rect);
    void setEventHandler(EventHandler *handler) {
        _event_handler = handler;
    }
    void addPropertyListener(PropertyListener *listener);
    void removePropertyListener(PropertyListener *listener);

    void requestClose() {
        _client_backend->requestClose();
    }
    void setFocus() {
        _client_backend->setFocus();
    }
    bool isDialog() const {
        return _client_backend->isDialog();
    }
    bool isModal() const {
        return _client_backend->isModal();
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

    virtual void draw(Canvas */*canvas*/) override {}
    virtual void handleButtonPress(int x_global, int y_global, int button) override;
    virtual void handleFocusChanged(bool has_focus) override;
    virtual void handleMap() override;
    virtual void handlePropertyChanged(ClientBackend::Property property) override;

    ClientBackend *_client_backend = 0;
    Workspace *_workspace = 0;
    EventHandler *_event_handler = 0;
    std::list<PropertyListener*> _property_listeners;
};

#endif // __CLIENT_H__
