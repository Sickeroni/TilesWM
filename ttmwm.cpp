#include "x11/x11_application.h"
#include "frontend_base.h"

#include "common.h"

class FrontendDummy : public FrontendBase
{
public:
    virtual void init() {}
    virtual void shutdown() {}
    virtual void focusActiveClient() {}
    virtual ClientFrontend *createClientFrontend(ClientBackend *backend, bool is_floating) { return 0; }
    virtual ClientFrontend *activeClient() { return 0; }
//     virtual void handleUnmanagedClientGotFocus() {}
//     virtual void handleUnmanagedClientLostFocus() {}
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
