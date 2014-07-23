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

    void requestClose() {
        _client_backend->requestClose();
    }
    void setFocus() {
        _client_backend->setFocus();
    }
    void grabMouseButton(int button) {
        _client_backend->grabMouseButton(button);
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
};

#endif // __CLIENT_H__
