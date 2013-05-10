#ifndef __X11_CLIENT_H__
#define __X11_CLIENT_H__

#include "client.h"

#include "x11_server_widget.h"
#include "rect.h"
#include "metrics.h"

#include <X11/Xlib.h>

#include <map>


class ClientContainer;
class X11ClientWidget;
class Icon;
class X11Icon;


//FIXME TODO - frame: draw focus

class X11Client : public Client, public X11ServerWidget::EventHandler
{
public:
    static void init();
    static bool handleEvent(const XEvent &ev);

    virtual ~X11Client();

    virtual void setFocus();
    virtual void setRect(const Rect &rect);
    virtual void setContainer(ClientContainer *container);
    virtual void raise();
    virtual Icon *icon();

    // X11ServerWidget::EventHandler implementation
    virtual void handleExpose();

    void map();
    void unmap();

private:
    class CriticalSection;

    enum WindowType { NORMAL, DIALOG, OVERRIDE_REDIRECT };

    static const int _inner_frame_width = Metrics::CLIENT_INNER_FRAME;

    static X11Client *find(Window wid);
    static void create(Window wid);
    static Atom getAtomProperty(Window wid, Atom property);

    X11Client();

//     bool validate();
    void mapInt();
    void unmapInt();
    bool refreshMapState();
    void refreshSizeHints();
    void refreshName();
    void refreshClass();
    void refreshFocusState();
    void refreshWindowType();
    void refreshIcon();
    void handleConfigureRequest(const XConfigureRequestEvent &ev);
    void drawFrame();
    bool isOverrideRedirect() {
        return _window_type == OVERRIDE_REDIRECT;
    }
    bool isDialog() {
        return _window_type == DIALOG;
    }

    // TODO - use hash
    static std::map<Window, X11Client*> _wid_index;
    static Atom _net_wm_window_type;
    static Atom _net_wm_window_type_dialog;
    static Atom _net_wm_icon;
    static Atom _kde_net_wm_window_type_override;

    X11ClientWidget *_widget;
    X11ServerWidget *_frame;
    X11Icon *_icon;
    int _max_width, _max_height;
    std::string _x11_name;
    std::string _x11_class;
    WindowType _window_type;
    bool _is_modal;
};

#endif // __X11_CLIENT_H__
