#ifndef __X11_CLIENT_H__
#define __X11_CLIENT_H__

#include "client.h"

#include "rect.h"

#include <X11/Xlib.h>


class ClientContainer;
class X11Widget;
class X11ServerWidget;


class X11Client : public Client
{
    //TODO use a hash here
//     static std::map<Window, X11Client*> _client_from_window;

public:
    virtual ~X11Client();

    virtual bool isMapped();
    virtual void setRect(const Rect &rect);
    virtual void setContainer(ClientContainer *container);

//     static void init();
//     static void shutdown();
//     static void newClient(Window wid);
//     static void widgetDestroyed(X11Widget *widget);
//     static void mapNotify(Window window);
//     static void unmapNotify(Window window);

    void onMapStateChanged();
    void onWidgetDestroyed();


    static void newClient(Window wid);

private:
    X11Client();
    bool validate();

    X11Widget *_widget;
    X11ServerWidget *_frame;
};

#endif // __X11_CLIENT_H__
