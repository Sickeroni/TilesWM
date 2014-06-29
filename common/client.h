#ifndef __CLIENT_H__
#define __CLIENT_H__

#include "child_widget.h"
#include "client_backend.h"

#include <string>

class Icon;
class Canvas;
class Workspace;
class Container;

class Client : public ChildWidget, public ClientBackend::EventHandler
{
public:
    Client(WidgetBackend *backend, bool is_floating);
    virtual ~Client();

    void setFocus();
    bool hasFocus();
    bool hasDecoration();
    const std::string &name();
    Icon *icon();

private:
    virtual void handleGeometryChanged(const Rect &rect) override;
    virtual void handleFocusChanged() override;
    virtual void handleMap() override;
//     virtual void handleUnmap() override;
    virtual void handleSizeHintsChanged() override;

};

#endif // __CLIENT_H__
