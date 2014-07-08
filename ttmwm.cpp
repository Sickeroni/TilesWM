// #define FRONTEND_DUMMY

#include "backend_x11/x11_application.h"
#ifndef FRONTEND_DUMMY
#include "application.h"
#include "mode_simple.h"
#include "mode_default.h"
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
    std::vector<Mode*> modes;

#ifdef FRONTEND_DUMMY
    FrontendDummy frontend(&modes);
#else
//     modes.push_back(new ModeDefault());
//     modes.push_back(new Mode3Panel());
    modes.push_back(new ModeSimple());
    Application frontend(&modes);
#endif

    X11Application backend(&frontend);

    if (!backend.init())
        return 1;

    backend.eventLoop();

    cout << "returned from main loop - shutting down ...\n";

    backend.shutdown();

#ifndef FRONTEND_DUMMY
    for(size_t i = 0; i < modes.size(); i++)
        delete modes[i];
    modes.clear();
#endif

    cout << "shutdown finished. goodbye.\n";

    return 0;
}
