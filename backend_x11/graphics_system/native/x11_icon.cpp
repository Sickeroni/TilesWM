#include "x11_icon.h"

#include "x11_canvas.h"
#include "common.h"

#include <X11/Xutil.h>


void X11Icon::init(Drawable parent)
{
   const unsigned int depth = 24; //FIXME HACK
    _pixmap = XCreatePixmap(dpy(), parent,
        make_unsigned<unsigned>(_width),
        make_unsigned<unsigned>(_height),
        depth);
#if 0
    if (_pixmap) {
        X11Canvas canvas(dpy(), _pixmap);

        Rect frame_rect(0, 0, _width, _height);

        canvas.fillRectangle(frame_rect, 0x0);

        canvas.drawFrame(frame_rect, 0xFFFFFF);
        canvas.drawLine(frame_rect.x, frame_rect.y,
                        frame_rect.x + frame_rect.w, frame_rect.y + frame_rect.h, 0xFFFFFF);
        canvas.drawLine(frame_rect.x, frame_rect.y + frame_rect.h,
                        frame_rect.x + frame_rect.w, frame_rect.y, 0xFFFFFF);
    }
#endif
}

X11Icon::X11Icon(
            Display *display,
            int width, 
            int height, 
            Drawable parent, 
            const unsigned long *argb_data, 
            uint32_t bg_color) :
    Icon(width, height),
    _dpy(display)
{
    init(parent);

    assert(_pixmap);

    if (XImage *image = XGetImage(dpy(), _pixmap,
                                  0, 0,
                                  make_unsigned<unsigned>(_width),
                                  make_unsigned<unsigned>(_height),
                                  0xFFFFFF,
                                  XYPixmap))
    {
        unsigned int red_bg = (bg_color & 0xFF0000) >> 16;
        unsigned int green_bg = (bg_color & 0x00FF00) >> 8;
        unsigned int blue_bg = bg_color & 0x0000FF;

        for(int y = 0; y< height; y++) {
            for(int x = 0; x < width; x++) {
                uint32_t pixel = static_cast<uint32_t>(argb_data[(y * width) + x]);

                unsigned int alpha = (pixel & 0xFF000000) >> 24;
                unsigned int red_in = (pixel & 0xFF0000) >> 16;
                unsigned int green_in = (pixel & 0x00FF00) >> 8;
                unsigned int blue_in = pixel & 0x0000FF;

                unsigned int red_out = ((red_in * alpha) / 256) + ((red_bg * (255 - alpha)) / 256);
                unsigned int green_out = ((green_in* alpha) / 256) + ((green_bg * (255 - alpha)) / 256);
                unsigned int blue_out = ((blue_in * alpha) / 256) + ((blue_bg * (255 - alpha)) / 256);

                pixel = (red_out << 16) | (green_out << 8) | blue_out;

                XPutPixel(image, x, y, pixel);
            }
        }

        X11Canvas canvas(dpy(), _pixmap);
        XPutImage(dpy(), _pixmap, canvas.gc(), image,
                0, 0, 0, 0,
                make_unsigned<unsigned>(_width),
                make_unsigned<unsigned>(_height));

        XDestroyImage(image);
    } else
        debug<<"XGetImage() failed.";
}


X11Icon::~X11Icon()
{
    if (_pixmap)
        XFreePixmap(dpy(), _pixmap);
//     if (_mask)
//         XFreePixmap(dpy(), _mask);
}
