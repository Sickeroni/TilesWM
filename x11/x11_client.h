#ifndef __X11_CLIENT_H__
#define __X11_CLIENT_H__

#include "client.h"

#include "rect.h"

#include <X11/Xlib.h>

#include <map>


class ClientContainer;
class X11ClientWidget;
class X11ServerWidget;

//FIXME TODO - frame: draw focus

class X11Client : public Client
{
public:
    virtual ~X11Client();

    virtual void setFocus();
    virtual void setRect(const Rect &rect);
    virtual void setContainer(ClientContainer *container);

    void map();

    static void init();
    static bool handleEvent(const XEvent &ev);

private:
    class CriticalSection;

    X11Client();

//     bool validate();
    void mapInt();
    void unmap();
    void unmapInt();
    bool refreshMapState();
    void refreshSizeHints();
    void refreshName();
    void refreshClass();
    void refreshFocusState();
    void refreshWindowType();
    void handleConfigureRequest(const XConfigureRequestEvent &ev);

    static X11Client *find(Window wid);
    static void handleCreate(Window wid);

    // TODO - use hash
    static std::map<Window, X11Client*> _wid_index;
    static Atom _net_wm_window_type;
    static Atom _net_wm_window_type_dialog;

    X11ClientWidget *_widget;
    X11ServerWidget *_frame;
    int _max_width, _max_height;
    std::string _x11_name;
    std::string _x11_class;
    bool _is_dialog;
    bool _is_modal;
};

#endif // __X11_CLIENT_H__
