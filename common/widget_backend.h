#ifndef __WIDGET_BACKEND_H__
#define __WIDGET_BACKEND_H__

#include "rect.h"

class Widget;
class ClientBackend;

class WidgetBackend
{
public:
//     enum Type {
//         CLIENT,
//         SERVER
//     };

    virtual ~WidgetBackend() {}

    virtual void setRect(const Rect &rect) = 0;
    virtual void setMapped(bool mapped) = 0;
    virtual void reparent(WidgetBackend *parent) = 0;
    virtual void redraw() = 0;
    virtual int maxTextHeight() const = 0;
    virtual void setFrontend(Widget *frontend) = 0;

    // if this widget is a client return its backend interface
    virtual ClientBackend *clientBackend() { return 0; }
    virtual void setMinimized(bool minimized) {}
    // intended as a rendering hint -
    // if set, only child needs to be rendered
    virtual void setMappedChild(WidgetBackend *child) {}


//     Type type() { return _type; }

// protected:
//     WidgetBackend(Type type) : _type(type) {}
//     WidgetBackend();

// private:
// //     const Type _type;
//     Widget * const _frontend = 0;
};

#endif