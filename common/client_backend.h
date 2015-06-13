#ifndef __CLIENT_BACKEND_H__
#define __CLIENT_BACKEND_H__

#include "rect.h"
#include "size_hints.h"

#include <string>

class Icon;
class WidgetBackend;

class ClientBackend
{
public:
    enum Property {
        PROP_NONE,
        PROP_SIZE_HINTS,
        PROP_REQUESTED_RECT,
        PROP_NAME,
        PROP_ICON_NAME,
        PROP_CLASS,
        PROP_ICON
    };

    virtual WidgetBackend *widget() = 0;

    virtual const Rect &rect() const = 0;
    virtual const Rect &requestedRect() const = 0;
    virtual bool hasFocus() const = 0;
    virtual void setFocus() = 0;
    virtual Icon *icon() const = 0;
    virtual void requestClose() = 0;
    virtual void grabMouseButton(int button) = 0;
    virtual bool isDialog() = 0;
    virtual bool isModal() = 0;

    virtual const std::string &name() const = 0;
    virtual const std::string &className() const = 0;
    virtual const std::string &iconName() const = 0;
    virtual const std::string &title() const = 0;
    virtual const SizeHints &sizeHints() const = 0;
};

#endif
