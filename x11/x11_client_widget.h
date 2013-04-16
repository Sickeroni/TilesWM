#ifndef __X11_CLIENT_WIDGET_H__
#define __X11_CLIENT_WIDGET_H__

#include "x11_widget.h"


class X11Client;


class X11ClientWidget : public X11Widget
{
public:
    X11ClientWidget(Window wid);
    virtual ~X11ClientWidget();

    static void newClientWidget(Window wid);
//     static void destroy(Window window);
    X11Client *client() { return _client; }

protected:
    virtual void onMapStateChanged();

private:

    X11Client *_client;
};


#endif // __X11_CLIENT_WIDGET_H__
