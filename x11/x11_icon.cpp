
#include "x11_icon.h"

#include "x11_canvas.h"
#include "x11_widget.h"
#include "x11_global.h"

#include <X11/Xutil.h>

#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>


using namespace X11Global;


void X11Icon::init(X11Widget *parent)
{
   const unsigned int depth = 24; //FIXME HACK
    _pixmap = XCreatePixmap(dpy(), parent->wid(), _width, _height, depth);
    if (_pixmap) {
        X11Canvas canvas(_pixmap);

        Rect frame_rect(0, 0, _width - 1, _height - 1);

        canvas.fillRectangle(frame_rect, 0x0);

        canvas.drawFrame(frame_rect, 0xFFFFFF);
        canvas.drawLine(frame_rect.x, frame_rect.y,
                        frame_rect.x + frame_rect.w, frame_rect.y + frame_rect.h, 0xFFFFFF);
        canvas.drawLine(frame_rect.x, frame_rect.y + frame_rect.h,
                        frame_rect.x + frame_rect.w, frame_rect.y, 0xFFFFFF);
    }
}


X11Icon::X11Icon(int width, int height, X11Widget *parent) : Icon(width, height),
    _pixmap(0),
    _mask(0)
{
    init(parent);
}



X11Icon::X11Icon(int width, int height, X11Widget *parent, const unsigned long *argb_data, unsigned long bg_color) : Icon(width, height),
    _pixmap(0),
    _mask(0)
{
    init(parent);

    if (XImage *image = XGetImage(dpy(), _pixmap,
                                  0, 0,
                                  _width, _height,
                                  0xFFFFFF,
                                  XYPixmap))
    {
        unsigned int red_bg = (bg_color & 0xFF0000) >> 16;
        unsigned int green_bg = (bg_color & 0x00FF00) >> 8;
        unsigned int blue_bg = bg_color & 0x0000FF;

        for(int y = 0; y< height; y++) {
            for(int x = 0; x < width; x++) {
                unsigned long pixel = argb_data[(y *width) + x];

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

        X11Canvas canvas(_pixmap);
        XPutImage(dpy(), _pixmap, canvas.gc(), image,
                0, 0, 0, 0, _width, _height);

        XDestroyImage(image);
    } else
        std::cerr<<"XGetImage() failed.\n";
}


X11Icon::~X11Icon()
{
    if (_pixmap)
        XFreePixmap(dpy(), _pixmap);
    if (_mask)
        XFreePixmap(dpy(), _mask);
}
