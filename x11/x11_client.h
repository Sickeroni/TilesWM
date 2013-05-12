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


//FIXME TODO - set border width via XConfigureWindow

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
    // values for the WM_STATE property
    enum WmState { STATE_WITHDRAWN = 0, STATE_NORMAL = 1, STATE_ICONIC = 3 };

    static const int _inner_frame_width = Metrics::CLIENT_INNER_FRAME_MARGIN; //FIXME remove
    static const int _titlebar_gap = 4;

    static X11Client *find(Window wid);
    static X11Client *findByFrame(Window wid);
    static void create(Window wid);
    static Atom getAtomProperty(Window wid, Atom property);
    static void cancelDrag();
    static void finishDrag();

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
    bool isFloating() {
        return _is_modal || isDialog() || isOverrideRedirect();
    }
    void calcFrameMargins(int &side, int &top, int &bottom);
    void calcFrameRect(const Rect &client_rect, Rect &frame_rect);
    void calcClientRect(const Rect &frame_rect, Rect &client_rect);
    void startDrag(int x, int y);

    // TODO - use hash
    static std::map<Window, X11Client*> _wid_index;
    static std::map<Window, X11Client*> _frame_wid_index;
    static X11Client *_dragged;
    static int _dragged_original_x, _dragged_original_y;
    static int _drag_start_x, _drag_start_y;

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
