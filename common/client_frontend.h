#ifndef __CLIENT_FRONTEND_H__
#define __CLIENT_FRONTEND_H__

#include "client_backend.h"

class ClientFrontend
{
public:
    virtual bool isFloating() = 0;
    virtual void handleGeometryChanged(const Rect &rect) = 0;
    virtual void handleFocusChanged(bool has_focus) = 0;
    virtual void handleMap() = 0;
//         virtual void handleUnmap() = 0;
    virtual void handlePropertyChanged(ClientBackend::Property property) = 0;

//     virtual void manage() = 0;
//     virtual void unmanage() = 0;
};

#endif
