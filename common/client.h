#ifndef __CLIENT_H__
#define __CLIENT_H__


#include "list.h"

#include "rect.h"

#include <map>
#include <string>

// class Widget;
class ClientContainer;

class Client : public List<Client>::Item
{
public:
//     static void startup();
//     static void shutdown();
//     static void createClient(Window window, Display *dpy);
//     static void windowDestroyed(Window window);
//     static void mapNotify(Window window);
//     static void unmapNotify(Window window);

    virtual ~Client();

    virtual bool isMapped() = 0;
    virtual void setRect(const Rect &rect) = 0;

    virtual void setContainer(ClientContainer *c);

    ClientContainer *container() {
        return _container;
    }

    const std::string &name() { return _name; }

    void onMapStateChanged();


protected:
    Client();

    std::string _name;

private:

//     Window _window;
//     Widget *_widget;
    ClientContainer *_container;
//     Display *_dpy;
//     bool _is_mapped;
};

#endif // __CLIENT_H__
