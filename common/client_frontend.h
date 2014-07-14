#ifndef __CLIENT_FRONTEND_H__
#define __CLIENT_FRONTEND_H__

#include "client_backend.h"
#include "rect.h"

class ClientFrontend
{
public:
    virtual void handleFocusChanged(bool has_focus) = 0;
    virtual void handleMap() = 0;
    virtual void handlePropertyChanged(ClientBackend::Property property) = 0;
    virtual void handleConfigureRequest(const Rect &rect) = 0;
};

#endif
