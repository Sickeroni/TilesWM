#ifndef __X11__SERVER_WIDGET_H__
#define __X11__SERVER_WIDGET_H__

#include "x11_widget.h"

#include <X11/Xlib.h>


// class X11EventHandler
// {
//     virtual void HandleEvent(const XEvent &event) = 0;
// };


class X11ServerWidget : public X11Widget
{
public:
    static X11ServerWidget *create(X11ServerWidget *parent);

    virtual ~X11ServerWidget();

private:
    X11ServerWidget(Window wid);
//     GC _gc;
//     X11EventHandler *_eventHandler;
};

#endif // __X11__SERVER_WIDGET_H__
