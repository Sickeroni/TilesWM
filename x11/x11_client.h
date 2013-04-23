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

    static void init();
    static void handleCreate(Window wid);
    static void handleDestroy(X11Widget *widget);
    static void handleUnmap(X11Widget *widget);
    static void handleMapRequest(X11Widget *widget);
    static void handleConfigureRequest(X11Widget *widget, const XConfigureRequestEvent &ev);

private:
    X11Client();
    bool validate();
    void map();
    void unmap();

    X11Widget *_widget;
    X11ServerWidget *_frame;
    int _max_width, _max_height;
};

#endif // __X11_CLIENT_H__
