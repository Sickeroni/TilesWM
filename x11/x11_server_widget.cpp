#include "x11_server_widget.h"

#include "x11_application.h"
#include "x11_canvas.h"

#include <iostream>
#include <stdlib.h>
#include <string.h>


X11ServerWidget::X11ServerWidget(Window wid) : X11Widget(wid, SERVER),
    _canvas(new X11Canvas(wid))
{
}

X11ServerWidget::~X11ServerWidget()
{
    delete _canvas;
    _canvas = 0;
    XDestroyWindow(X11Application::display(), wid());
}

X11ServerWidget *X11ServerWidget::create(X11ServerWidget *parent)
{
    std::cout << "X11ServerWidget *X11ServerWidget::create(X11ServerWidget *parent)\n";
    Window parent_wid = parent ? parent->wid() : X11Application::root();

    std::cout << "parent_wid: " << parent_wid << '\n';


    const unsigned long fg = 0xFFFFFF;
//     connst unsigned long bg = 0xFF4444;
    const unsigned long bg = 0x999999;

    Window wid = XCreateSimpleWindow(X11Application::display(),
                                     parent_wid,
                                     0, 0,
                                     100, 100,
                                     0,
                                     fg,
                                     bg);
    if (!wid)
        abort();

    //FIXME UGLY - use XCreateWindow()
    {
        XWindowAttributes attr;
        XSetWindowAttributes new_attr;

        if (!XGetWindowAttributes(X11Application::display(), wid, &attr))
            abort();

        memset(&new_attr, 0, sizeof(XSetWindowAttributes));

        new_attr.event_mask = attr.your_event_mask | SubstructureNotifyMask;

        //FIXME reset all attributes ?

        XChangeWindowAttributes(X11Application::display(), wid, CWEventMask, &new_attr);
    }

    return new X11ServerWidget(wid);
}
