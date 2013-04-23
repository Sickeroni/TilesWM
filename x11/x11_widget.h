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

    static void createNotify(const XCreateWindowEvent &ev);
    static void destroyNotify(const XDestroyWindowEvent &ev);
    static void unmapNotify(const XUnmapEvent &ev);
    static void mapRequest(const XMapRequestEvent &ev);
    static void configureRequest(const XConfigureRequestEvent &ev);
    static X11Widget *find(Window wid);

    virtual ~X11Widget();

    virtual bool validate() { return !_is_destroyed; }
    virtual void setRect(const Rect &rect);

    Type type() { return _type; }
//     const Rect &rect() { return _rect; }

    void move(int x, int y);
    bool isMapped() { return _is_mapped; }
    Window wid() { return _wid; }
    void reparent(X11ServerWidget *new_parent);
    void map();
    void unmap();


protected:
    X11Widget(Window wid, Type type);

    bool _is_destroyed;
    bool _is_mapped;

private:
    static std::map<Window, X11Widget*> _wid_index;

    Window _wid;
    Type _type;
//     Rect _rect;
};

#endif


#endif // __X11_WIDGET_H__
