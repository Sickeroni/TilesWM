#ifndef __CLIENT_BACKEND_H__
#define __CLIENT_BACKEND_H__

#include "rect.h"

#include <string>

class Icon;

class ClientBackend
{
public:
    enum Property {
        PROP_NONE,
        PROP_SIZE_HINTS,
        PROP_NAME,
        PROP_ICON_NAME,
        PROP_CLASS,
        PROP_ICON
    };

    class EventHandler {
    public:
        virtual void handleGeometryChanged(const Rect &rect) = 0;
        virtual void handleFocusChanged() = 0;
        virtual void handleMap() = 0;
//         virtual void handleUnmap() = 0;
        virtual void handlePropertyChanged(Property property) = 0;
    };

    virtual void setEventHandler(EventHandler *handler) = 0;
    virtual const Rect &rect() = 0;
    virtual bool hasFocus() = 0;
    virtual void setFocus() = 0;
    virtual Icon *icon() = 0;
    virtual void requestClose() = 0;
    virtual void raise() = 0;

    virtual const std::string &name() = 0;
//     virtual const std::string &className() = 0;
//     virtual const std::string &iconName() = 0; 
//     virtual const std::string &title() = 0; 
//     virtual int minWidth() = 0; 
//     virtual int minHeight() = 0; 
//     virtual int maxWidth() = 0;
//     virtual int maxHeight() = 0; 
};

#endif
