#ifndef __CLIENT_H__
#define __CLIENT_H__

#include <X11/Xlib.h>

#include <map>

class Container;

class Client
{
    Client(Window w, Display *dpy, bool is_mapped);

    static std::map<Window, Client*> _client_from_window;
    Window _window;
    Container *_container;
    Display *_dpy;
    bool _is_mapped;

public:
    static void startup();
    static void shutdown();
    static void createClient(Window window, Display *dpy);
    static void windowDestroyed(Window window);
    static void mapNotify(Window window);
    static void unmapNotify(Window window);

    Container *container() {
        return _container;
    }
    void setContainer(Container *c);
    void setRect(int x, int y, int w, int h);
    bool isMapped() { return _is_mapped; }
};

#endif // __CLIENT_H__
