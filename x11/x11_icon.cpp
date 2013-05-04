#include "x11_icon.h"

#include "x11_canvas.h"
#include "x11_widget.h"
#include "x11_application.h"

X11Icon::X11Icon(int width, int height, X11Widget *parent) : Icon(width, height),
    _pixmap(0),
    _mask(0)
{
    const unsigned int depth = 24; //FIXME HACK
    _pixmap = XCreatePixmap(X11Application::display(), parent->wid(), _width, _height, depth);
    if (_pixmap) {
        X11Canvas canvas(_pixmap);

        Rect frame_rect(0, 0, width - 1, height - 1);

        canvas.fillRectangle(frame_rect, 0x0);

        canvas.drawFrame(frame_rect, 0xFFFFFF);
        canvas.drawLine(frame_rect.x, frame_rect.y,
                        frame_rect.x + frame_rect.w, frame_rect.y + frame_rect.h, 0xFFFFFF);
        canvas.drawLine(frame_rect.x, frame_rect.y + frame_rect.h,
                        frame_rect.x + frame_rect.w, frame_rect.y, 0xFFFFFF);
    }
}

X11Icon::~X11Icon()
{
    XFreePixmap(X11Application::display(), _pixmap);
    if (_mask)
        XFreePixmap(X11Application::display(), _mask);
}
