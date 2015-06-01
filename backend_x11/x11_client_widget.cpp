#include "x11_client_widget.h"

#include "x11_client.h"
#include "x11_server_widget.h"
#include "x11_global.h"
#include "common.h"


using namespace X11Global;


X11ClientWidget::X11ClientWidget(
            Window wid,
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
