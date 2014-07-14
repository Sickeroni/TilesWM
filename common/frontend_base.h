#ifndef __FRONTEND_BASE_H__
#define __FRONTEND_BASE_H__

class ClientFrontend;
class ClientBackend;
class KeyGrabHandlerBase;
class Backend;

class FrontendBase
{
public:
    virtual void init(Backend *backend) = 0;
    virtual void shutdown() = 0;
    virtual void focusActiveClient() = 0;
//     virtual void manageClient(WidgetBackend *backend, bool is_floating);
    virtual ClientFrontend *createClientFrontend(ClientBackend *backend) = 0;
    virtual void destroyClientFrontend(ClientFrontend *frontend) = 0;
//     virtual ClientFrontend *activeClient() = 0;

    virtual int numKeyGrabHandlers() = 0;
    virtual KeyGrabHandlerBase *keyGrabHandler(int index) = 0;

//     virtual void handleUnmanagedClientGotFocus() = 0;
//     virtual void handleUnmanagedClientLostFocus() = 0;
};

#endif
