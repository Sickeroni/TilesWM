#ifndef __CLIENT_H__
#define __CLIENT_H__

#include "child_widget.h"
#include "client_backend.h"
#include "client_frontend.h"
#include "common.h"

#include <string>

class Icon;

class Client final : public ChildWidget, public ClientFrontend
{
public:
    Client(ClientBackend *client_backend, bool is_floating);
    ~Client();

    virtual bool isFloating() override { return _is_floating; }
    virtual bool hasDecoration() override {
        return isFloating();
    }
    void setIsFloating(bool is_floating) {
        _is_floating = is_floating;
        _client_backend->updateGeometry();
    }
    const std::string &name() {
        return _client_backend->name();
    }
    const std::string &className() {
        return _client_backend->className();
    }
    const std::string &iconName() {
        return _client_backend->iconName();
    }
    const std::string &title() {
        return _client_backend->title();
    }
    Icon *icon() {
        return _client_backend->icon();
    }
    void requestClose() {
        _client_backend->requestClose();
    }
    void setFocus() {
        _client_backend->setFocus();
    }
    bool hasFocus() {
        return _client_backend->hasFocus();
    }
    void raise() {
        _client_backend->raise();
    }
    int minWidth() {
        return _client_backend->minWidth();
    }
    int minHeight() {
        return _client_backend->minHeight();
    }
    int maxWidth() {
        return _client_backend->maxWidth();
    }
    int maxHeight() {
        return _client_backend->maxHeight();
    }

private:
    virtual void handleGeometryChanged(const Rect &rect) override;
    virtual void handleFocusChanged(bool has_focus) override;
    virtual void handleMap() override;
    virtual void handlePropertyChanged(ClientBackend::Property property) override;

    void handleSizeHintsChanged();

    bool _is_floating = false;
    ClientBackend *_client_backend = 0;
};

#endif // __CLIENT_H__
