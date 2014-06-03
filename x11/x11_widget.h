#ifndef __X11_WIDGET_H__
#define __X11_WIDGET_H__

#include "rect.h"

#include <X11/Xlib.h>

class X11ServerWidget;

class X11Widget
{
public:
    enum Type { SERVER, CLIENT };

    static bool handleEvent(const XEvent &ev);

    virtual ~X11Widget();

    Type type() { return _type; }
    const Rect &rect() { return _rect; }

    void configure(unsigned int value_mask, const XWindowChanges &changes);
    void setRect(const Rect &rect);
    void move(int x, int y);
    bool isMapped() { return _is_mapped; }
    Window wid() { return _wid; }
    void reparent(X11ServerWidget *new_parent, int x = 0, int y = 0);
    void map();
    void unmap();

protected:
    X11Widget(Window wid, Type type, bool is_mapped, const Rect &rect);

    Rect _rect;
    bool _is_mapped;

private:
    Window _wid;
    Type _type;
};

#endif // __X11_WIDGET_H__
