#ifndef __X11_CLIENT_WIDGET_H__
#define __X11_CLIENT_WIDGET_H__

#include "x11_widget.h"


class X11Client;


class X11ClientWidget : public X11Widget
{
public:
    X11ClientWidget(Window wid, bool is_mapped, bool is_viewable, const Rect &rect);
    ~X11ClientWidget();

    void configure(unsigned int value_mask, const XWindowChanges &changes);

    bool refreshMapState();

    bool isViewable() { return _is_viewable; }

private:
    bool _is_viewable = false;
};


#endif // __X11_CLIENT_WIDGET_H__
