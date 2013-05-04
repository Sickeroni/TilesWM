#ifndef __CLIENT_H__
#define __CLIENT_H__


#include "list.h"

#include "rect.h"

#include <map>
#include <string>

// class Widget;
class ClientContainer;
class Icon;

class Client : public List<Client>::Item
{
//FIXME
//TODO cache size, pos
public:
//     static void startup();
//     static void shutdown();
//     static void createClient(Window window, Display *dpy);
//     static void windowDestroyed(Window window);
//     static void mapNotify(Window window);
//     static void unmapNotify(Window window);

    virtual ~Client();

    virtual void setRect(const Rect &rect) = 0;
    virtual void setFocus() = 0;
    virtual void raise() = 0;
    virtual Icon *icon() = 0;

    virtual void setContainer(ClientContainer *c);

    ClientContainer *container() { return _container; }
    bool isMapped() { return _is_mapped; }
    bool hasFocus() { return _has_focus; }
    const std::string &name() { return _name; }

protected:
    Client(bool is_mapped);

    bool _is_mapped;
    bool _has_focus;
    std::string _name;

private:

//     Window _window;
//     Widget *_widget;
    ClientContainer *_container;
//     Display *_dpy;
};

#endif // __CLIENT_H__
