#ifndef __CLIENT_H__
#define __CLIENT_H__

#include "child_widget.h"
#include "client_backend.h"

#include <string>

class Icon;
class Canvas;
class Workspace;
class Container;

class Client : public ChildWidget
{
public:
    Client(bool is_floating);

    void setFocus();
    bool hasFocus();
    bool hasDecoration();
    const std::string &name();
    Icon *icon();
    void requestClose();

private:
    struct : public ClientFrontend {
        virtual void foo() { client->setFocus();}
        virtual void bar() { client->requestClose(); }

        Client *client = 0;
    } _frontend_impl;

    ~Client();

    virtual void handleGeometryChanged(const Rect &rect) override;
    virtual void handleFocusChanged() override;
    virtual void handleMap() override;
//     virtual void handleUnmap() override;
    virtual void handlePropertyChanged(ClientBackend::Property property) override;

    void handleSizeHintsChanged();

};

#endif // __CLIENT_H__
