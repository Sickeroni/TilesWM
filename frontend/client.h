#ifndef __CLIENT_H__
#define __CLIENT_H__

#include "child_widget.h"
#include "client_backend.h"
#include "client_frontend.h"

#include <string>

class Icon;

class Client : public ChildWidget, public ClientFrontend
{
public:
    Client(ClientBackend *client_backend, bool is_floating);

    virtual bool isFloating() { return true; }

    void setFocus();
    bool hasFocus();
    bool hasDecoration();
    const std::string &name();
    Icon *icon();
    void requestClose();

private:
    ~Client();

    virtual void handleGeometryChanged(const Rect &rect) override;
    virtual void handleFocusChanged(bool has_focus) override;
    virtual void handleMap() override;
    virtual void handlePropertyChanged(ClientBackend::Property property) override;

    void handleSizeHintsChanged();

    ClientBackend *_client_backend = 0;
};

#endif // __CLIENT_H__
