#ifndef __X11__SERVER_WIDGET_H__
#define __X11__SERVER_WIDGET_H__

#include "x11_widget.h"

#include <X11/Xlib.h>

#include <map>


class X11Canvas;

// class X11EventHandler
// {
//     virtual void HandleEvent(const XEvent &event) = 0;
// };


class X11ServerWidget : public X11Widget
{
public:
    static X11ServerWidget *create(X11ServerWidget *parent);
    static bool handleEvent(const XEvent &ev);

    static bool isServerWidget(Window wid) {
        return find(wid);
    }

    virtual ~X11ServerWidget();

    X11Canvas *canvas() { return _canvas; }

private:
    X11ServerWidget(Window wid);

    static X11ServerWidget *find(Window wid);

    X11Canvas *_canvas;
//     GC _gc;
//     X11EventHandler *_eventHandler;
    static std::map<Window, X11ServerWidget*> _wid_index;
};

#endif // __X11__SERVER_WIDGET_H__
