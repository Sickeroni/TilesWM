// #define FRONTEND_DUMMY

#include "backend_x11/x11_application.h"
#ifndef FRONTEND_DUMMY
#include "frontend/application.h"
#endif

#include "common.h"


#ifdef FRONTEND_DUMMY
class FrontendDummy : public FrontendBase
{
public:
    virtual void init() {}
    virtual void shutdown() {}
    virtual void focusActiveClient() {}
    virtual ClientFrontend *createClientFrontend(ClientBackend *backend, bool is_floating) {
//         return new ClientFrontendDummy(backend);
        return 0;
    }
    virtual void destroyClientFrontend(ClientFrontend *frontend) {}
    virtual int numKeyGrabHandlers() { return 0; }
    virtual KeyGrabHandlerBase *keyGrabHandler(int index) { return 0; }
};
#endif

int main()
{
#ifdef FRONTEND_DUMMY
    FrontendDummy frontend;
#else
    Application frontend;
#endif

    X11Application backend(&frontend);

    if (!backend.init())
        return 1;

    backend.eventLoop();

    cout << "returned from main loop - shutting down ...\n";

    backend.shutdown();

    cout << "shutdown finished. goodbye.\n";

    return 0;
}
