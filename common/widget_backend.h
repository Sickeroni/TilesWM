#ifndef __WIDGET_BACKEND_H__
#define __WIDGET_BACKEND_H__

#include "rect.h"

class WidgetFrontend;

class WidgetBackend
{
public:
    enum CursorType {
        CURSOR_DEFAULT,
        CURSOR_MOVE,
        CURSOR_RESIZE_BOTTOM_RIGHT
    };

    virtual ~WidgetBackend() {}

    virtual void setRect(const Rect &rect) = 0;
    virtual void setMapped(bool mapped) = 0;
    virtual void reparent(WidgetBackend *parent) = 0;
    virtual void redraw() = 0;
    virtual int maxTextHeight() const = 0;
    //FIXME remove
    virtual void setFrontend(WidgetFrontend *frontend) = 0;
    virtual void grabMouse(CursorType cursor) = 0;
    virtual void releaseMouse() = 0;
    virtual void raise() = 0;

    // if this widget is a client return its backend interface
//     virtual ClientBackend *clientBackend() { return 0; }


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
