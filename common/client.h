#ifndef __CLIENT_H__
#define __CLIENT_H__


#include "list.h"

#include "rect.h"

#include <map>
#include <string>


// class Widget;
class ClientContainer;
class Icon;
class Canvas;


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
    static const int _titlebar_gap = 4;

    Client(bool is_mapped);

    virtual const Rect &frameRect() = 0;
    virtual bool hasDecoration() = 0;
    virtual int maxTextHeight() = 0;

    void calcFrameRect(const Rect &client_rect, Rect &frame_rect);
    void calcClientRect(const Rect &frame_rect, Rect &client_rect);
    void drawFrame(Canvas *canvas);

    bool _is_mapped;
    bool _has_focus;
    std::string _name;

private:
    void calcFrameMargins(int &side, int &top, int &bottom);

//     Window _window;
//     Widget *_widget;
    ClientContainer *_container;
//     Display *_dpy;
};

#endif // __CLIENT_H__
