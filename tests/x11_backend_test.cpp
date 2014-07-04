#include "../backend_x11/x11_application.h"
#include "frontend_base.h"
#include "client_frontend.h"
#include "widget_backend.h"

#include "common.h"

class ClientFrontendDummy final : public ClientFrontend
{
public:
    ClientFrontendDummy(ClientBackend *backend): _backend(backend) {}

    virtual bool isFloating() { return true; }
    virtual void handleGeometryChanged(const Rect &rect) {}
    virtual void handleFocusChanged(bool has_focus) {}
    virtual void handleMap() {
        _backend->widget()->setMapped(true);
    }
    virtual void handlePropertyChanged(ClientBackend::Property property) {}

private:
    ClientBackend *_backend = 0;
};


class FrontendDummy : public FrontendBase
{
public:
    virtual void init() {}
    virtual void shutdown() {}
    virtual void focusActiveClient() {}
    virtual ClientFrontend *createClientFrontend(ClientBackend *backend, bool is_floating) { 
        return new ClientFrontendDummy(backend);
    }
    virtual void destroyClientFrontend(ClientFrontend *frontend) {
        delete dynamic_cast<ClientFrontendDummy*>(frontend);
    }
    virtual int numKeyGrabHandlers() { return 0; }
    virtual KeyGrabHandlerBase *keyGrabHandler(int index) { return 0; }
};

int main()
{
    FrontendDummy dummy;

    X11Application app(&dummy);

    if (!app.init())
        return 1;

    app.eventLoop();

    cout << "returned from main loop - shutting down ...\n";

    app.shutdown();

    cout << "shutdown finished. goodbye.\n";

    return 0;
}
