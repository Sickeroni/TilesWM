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
    const SizeHints &sizeHints() {
        return _client_backend->sizeHints();
    }
    void applySizeHints(Rect &rect) {
        _client->applySizeHints(rect);
    }

    void setEventHandler(Client::EventHandler *handler);
    void setPropertyListener(Client::PropertyListener *listener);

    const Client *wrappedClient() { return _client; }

private:
    Client *_client = 0;
    const ClientBackend *_client_backend = 0;
    Client::EventHandler *_event_handler = 0;
    Client::PropertyListener *_property_listener = 0;
    WindowManager *_wm = 0;
};

#endif
