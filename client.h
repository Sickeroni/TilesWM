#ifndef __CLIENT_H__
#define __CLIENT_H__

#include "container_element.h"


#include <X11/Xlib.h>

#include <map>

class ClientContainer;

class Client : public ContainerElement
{
    Client(Window w, Display *dpy, bool is_mapped);

    //TODO use a hash here
    static std::map<Window, Client*> _client_from_window;

    Window _window;
    ClientContainer *_container;
    Display *_dpy;
    bool _is_mapped;

public:
    static void startup();
    static void shutdown();
    static void createClient(Window window, Display *dpy);
    static void windowDestroyed(Window window);
    static void mapNotify(Window window);
    static void unmapNotify(Window window);

    ClientContainer *container() {
        return _container;
    }
    void setContainer(ClientContainer *c);
    void setRect(int x, int y, int w, int h);
    bool isMapped() { return _is_mapped; }
};

#endif // __CLIENT_H__
