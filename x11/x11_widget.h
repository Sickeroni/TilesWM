#ifndef __X11_WIDGET_H__
#define __X11_WIDGET_H__

#include "rect.h"

#include <map>
#include <X11/Xlib.h>

class X11ServerWidget;

#if 1

class X11Widget
{
public:
    enum Type { SERVER, CLIENT };

    static void initClientWidgets();
    static void createNotify(const XCreateWindowEvent &ev);
    static void destroyNotify(const XDestroyWindowEvent &ev);
    static void mapNotify(const XMapEvent &ev);
    static void unmapNotify(const XUnmapEvent &ev);

    virtual ~X11Widget();

    virtual bool validate() { return !_is_destroyed; }

    Type type() { return _type; }
//     const Rect &rect() { return _rect; }
    virtual void setRect(const Rect &rect);
    bool isMapped() { return _is_mapped; }
    Window wid() { return _wid; }
    void reparent(X11ServerWidget *new_parent);

protected:
    static X11Widget *find(Window wid);

    X11Widget(Window wid, Type type);

    virtual void onMapStateChanged() {}

    bool _is_destroyed;

private:
    static std::map<Window, X11Widget*> _wid_index;

    Window _wid;
    Type _type;
//     Rect _rect;
    bool _is_mapped;
};

#endif


#endif // __X11_WIDGET_H__
