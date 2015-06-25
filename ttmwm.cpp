// #define FRONTEND_DUMMY

#include "backend_x11/x11_application.h"
#ifndef FRONTEND_DUMMY
#include "application.h"
#include "mode.h"
// #include "mode_mixed.h"
#include "mode_simple.h"
// #include "mode_default.h"
#include "mode_3panel.h"
#include "themes/default/default_theme.h"
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
    FrontendDummy frontend(&modes);
#else
    std::vector<Mode*> modes;

//     modes.push_back(new ModeMixed(new ModeSimple(), new Mode3Panel()));
//     modes.push_back(new ModeMixed(new ModeSimple(), new ModeDefault()));
    modes.push_back(new ModeSimple());
//     modes.push_back(new ModeDefault());
    modes.push_back(new Mode3Panel());

    ThemeBackend *theme_backend = new DefaultTheme();

    Application frontend(&modes, theme_backend);
#endif

    X11Application backend(&frontend);

    int error = 0;

    if (backend.init()) {
        backend.eventLoop();
        cout << "returned from main loop - shutting down ...\n";
        backend.shutdown();
    } else
        error = 1;

#ifndef FRONTEND_DUMMY
    for(size_t i = 0; i < modes.size(); i++)
        delete modes[i];
    modes.clear();

    delete theme_backend;
    theme_backend = 0;
#endif

    cout << "shutdown finished. goodbye.\n";

    return error;
}
