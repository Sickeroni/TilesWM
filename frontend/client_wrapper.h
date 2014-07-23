#ifndef __CLIENT_WRAPPER_H__
#define __CLIENT_WRAPPER_H__

#include "client.h"
#include "child_widget.h"
#include "window_manager.h"

class ClientWrapper final : public ChildWidget
{
public:
    ClientWrapper(Client *client, WindowManager *wm);
    ~ClientWrapper();

    virtual void setRect(const Rect &rect) override;

    void handleWindowManagerIsActiveChanged();

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
        _client->requestClose();
    }
    void setFocus() {
        assert(_wm->isActive());
        _client->setFocus();
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
    void grabMouseButton(int button) {
        _client->grabMouseButton(button);
    }
    void applySizeHints(Rect &rect) {
        _client->applySizeHints(rect);
    }

    const Client *client() { return _client; }

private:
    Client *_client = 0;
    const ClientBackend *_client_backend = 0;
    WindowManager *_wm = 0;
};

#endif
