#ifndef __X11_CLIENT_WIDGET_H__
#define __X11_CLIENT_WIDGET_H__

#include "x11_widget.h"


class X11Client;


class X11ClientWidget : public X11Widget
{
public:
    X11ClientWidget(Window wid, X11Client *client, bool is_mapped, const Rect &rect);
    ~X11ClientWidget();

//     virtual bool validate();

//     X11Client *client() { return _client; }
    void configure(unsigned int value_mask, const XWindowChanges &changes);

    bool refreshMapState();

// private:
//     X11Client *_client;
};


#endif // __X11_CLIENT_WIDGET_H__
