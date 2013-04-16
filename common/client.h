#ifndef __CLIENT_H__
#define __CLIENT_H__

// #include "widget.h"

#include "list.h"

// #include <X11/Xlib.h>

#include <map>

// class Widget;
class ClientContainer;

class Client : public List<Client>::Item
{

//     Window _window;
//     Widget *_widget;
    ClientContainer *_container;
//     Display *_dpy;
//     bool _is_mapped;

public:
//     static void startup();
//     static void shutdown();
//     static void createClient(Window window, Display *dpy);
//     static void windowDestroyed(Window window);
//     static void mapNotify(Window window);
//     static void unmapNotify(Window window);

    virtual ~Client();

    ClientContainer *container() {
        return _container;
    }
    void setContainer(ClientContainer *c);

    void onMapStateChanged();

//     void setRect(int x, int y, int w, int h) {
//         _widget->setRect(x, y, w, h);
//     }
//     bool isMapped() { return _is_mapped; }
//     bool isMapped() { return _widget->isMapped(); }

protected:
    Client();
};

#endif // __CLIENT_H__
