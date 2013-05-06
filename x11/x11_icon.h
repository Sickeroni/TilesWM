#ifndef __X11_ICON_H__
#define __X11_ICON_H__

#include "icon.h"

#include <X11/Xlib.h>


class X11Widget;


class X11Icon : public Icon
{
public:
    X11Icon(int width, int height, X11Widget *parent);
    X11Icon(int width, int height, X11Widget *parent, const unsigned long *argb_data, unsigned long bg_color);
    X11Icon(const X11Icon &other);
    ~X11Icon();
    X11Icon &operator=(const X11Icon &other);

    Pixmap pixmap() { return _pixmap; }
    Pixmap mask() { return _mask; }

private:
    void init(X11Widget *parent);

    Pixmap _pixmap;
    Pixmap _mask;
};

#endif // __X11_ICON_H__
