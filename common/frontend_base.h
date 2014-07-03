#ifndef __FRONTEND_BASE_H__
#define __FRONTEND_BASE_H__

class ClientFrontend;
class ClientBackend;
class KeyGrabHandlerBase;

class FrontendBase
{
public:
    virtual void init() = 0;
    virtual void shutdown() = 0;
    virtual void focusActiveClient() = 0;
//     virtual void manageClient(WidgetBackend *backend, bool is_floating);
    virtual ClientFrontend *createClientFrontend(ClientBackend *backend, bool is_floating) = 0;
//     virtual ClientFrontend *activeClient() = 0;

    virtual int numKeyGrabHandlers() = 0;
    virtual KeyGrabHandlerBase *keyGrabHandler(int index) = 0;

//     virtual void handleUnmanagedClientGotFocus() = 0;
//     virtual void handleUnmanagedClientLostFocus() = 0;
};

#endif
