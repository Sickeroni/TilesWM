#ifndef __X11__SERVER_WIDGET_H__
#define __X11__SERVER_WIDGET_H__

#include "x11_widget.h"
#include "types.h"

#include <X11/Xlib.h>

#include <map>


class Canvas;

// class X11EventHandler
// {
//     virtual void HandleEvent(const XEvent &event) = 0;
// };


class X11ServerWidget : public X11Widget
{
public:
    struct EventHandler
    {
        virtual void handleExpose() = 0;
        virtual void handleButtonPress(const XButtonEvent &ev) {}
    };

    static X11ServerWidget *create(X11ServerWidget *parent, uint32 bg_color,
                                   EventHandler *event_handler, long event_mask);
    static bool handleEvent(const XEvent &ev);

    static bool isServerWidget(Window wid) {
        return find(wid);
    }

    virtual ~X11ServerWidget();

    Canvas *canvas() { return _canvas; }

private:
    X11ServerWidget(Window wid, EventHandler *event_handler, const Rect &rect);

    static X11ServerWidget *find(Window wid);

    Canvas *_canvas;
//     GC _gc;
    EventHandler *_event_handler;
    static std::map<Window, X11ServerWidget*> _wid_index;
};

#endif // __X11__SERVER_WIDGET_H__
