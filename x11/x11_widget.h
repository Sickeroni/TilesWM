#ifndef __X11_WIDGET_H__
#define __X11_WIDGET_H__

#include "rect.h"

#include <map>
#include <X11/Xlib.h>

#if 1

class X11Widget
{
public:
    enum Type { SERVER, CLIENT };

    static X11Widget *find(Window wid);
    static void createNotify(const XCreateWindowEvent &ev);
    static void destroyNotify(const XDestroyWindowEvent &ev);
    static void mapNotify(const XMapEvent &ev);
    static void unmapNotify(const XUnmapEvent &ev);

    virtual ~X11Widget();

    Type type() { return _type; }
    const Rect &rect() { return _rect; }
    void setRect(const Rect &rect);
    bool isMapped() { return _is_mapped; }

protected:
    X11Widget(Window wid, Type type);

    virtual void onMapStateChanged() {}

private:
    static std::map<Window, X11Widget*> _from_wid;

    Window _wid;
    Type _type;
    Rect _rect;
    bool _is_mapped;
};

#endif


#if 0
class X11Widget
{
public:
    static void startup();
    static void shutdown();
    static void create(Window window, Display *dpy);
    static void windowDestroyed(Window window);
    static void mapNotify(Window window);
    static void unmapNotify(Window window);
};
#endif

#endif // __X11_WIDGET_H__
