#include "x11_client_widget.h"

#include "x11_client.h"
#include "x11_server_widget.h"
#include "x11_global.h"
#include "common.h"


using namespace X11Global;


X11ClientWidget::X11ClientWidget(
            Window wid, X11Client *client,
            bool is_mapped,
            bool is_viewable,
            const Rect &rect) :
    X11Widget(wid, CLIENT, is_mapped, rect),
    _is_viewable(is_viewable)
{
}

X11ClientWidget::~X11ClientWidget()
{
}

#if 0
bool X11ClientWidget::validate()
{
    std::cout<<"X11ClientWidget::validate()\n";
    std::cout<<"_is_destroyed: "<<_is_destroyed<<'\n';

    if (_is_destroyed)
        return false;

    Window parent_wid = 0;
    if (_client->container())
        parent_wid = static_cast<X11ClientContainer*>(_client->container())->widget()->wid();
    else
        parent_wid = X11Application::root();

    XSync(dpy(), false);

    XEvent ev;


    if (XCheckTypedWindowEvent(dpy(), parent_wid, DestroyNotify, &ev)) {
        std::cout<<"got destroy notify.\n";
        std::cout<<"ev.xdestroywindow.window: "<<ev.xdestroywindow.window<<'\n';
        std::cout<<"_wid: "<<wid()<<'\n';
        if (ev.xdestroywindow.window == wid())
            _is_destroyed = true;
        XPutBackEvent(dpy(), &ev);
    }


    return !_is_destroyed;
}
#endif


bool X11ClientWidget::refreshMapState()
{
    XWindowAttributes attr;
    if (XGetWindowAttributes(dpy(), wid(), &attr)) {
        _is_mapped = (attr.map_state != IsUnmapped);
        _is_viewable = (attr.map_state == IsViewable);
        return true;
    } else {
        debug<<"XGetWindowAttributes() failed.";
        return false;
    }

}

void X11ClientWidget::configure(unsigned int value_mask, const XWindowChanges &changes)
{
    if (value_mask & CWX)
        _rect.x = changes.x;
    if (value_mask & CWY)
        _rect.y = changes.y;
    if (value_mask & CWWidth)
        _rect.w = changes.width;
    if (value_mask & CWHeight)
        _rect.h = changes.height;
    XConfigureWindow(dpy(), wid(), value_mask, const_cast<XWindowChanges*>(&changes));
}
