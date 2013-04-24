#ifndef __X11_CLIENT_H__
#define __X11_CLIENT_H__

#include "client.h"

#include "rect.h"

#include <X11/Xlib.h>

#include <map>


class ClientContainer;
class X11ClientWidget;
class X11ServerWidget;


class X11Client : public Client
{
public:
    virtual ~X11Client();

    virtual bool isMapped();
    virtual void setRect(const Rect &rect);
    virtual void setContainer(ClientContainer *container);

    void map();
    void mapInt();

    static void init();
    static bool handleEvent(const XEvent &ev);

private:
    class CriticalSection;

    X11Client();
//     bool validate();
    void unmap();
    void unmapInt();
    bool refreshMapState();

    static X11Client *find(Window wid);
    static void handleCreate(Window wid);
    static void handleConfigureRequest(X11Client *client, const XConfigureRequestEvent &ev);

    static std::map<Window, X11Client*> _wid_index;

    X11ClientWidget *_widget;
    X11ServerWidget *_frame;
    int _max_width, _max_height;
};

#endif // __X11_CLIENT_H__
