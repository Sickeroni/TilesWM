#ifndef __X11_ICON_H__
#define __X11_ICON_H__

#include "icon.h"
#include "common.h"

#include <X11/Xlib.h>


class X11Widget;


class X11Icon final : public Icon
{
public:
    X11Icon(Display *display, int width, int height, Drawable parent, const unsigned long *argb_data, uint32_t bg_color);
    X11Icon(const X11Icon &other);
    ~X11Icon();
    X11Icon &operator=(const X11Icon &other);

    Pixmap pixmap() { return _pixmap; }
    Display *dpy() { return _dpy; }

private:
    void init(Drawable parent);

    Pixmap _pixmap = None;
    Display *_dpy = 0;
};

#endif // __X11_ICON_H__
