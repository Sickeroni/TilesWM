#ifndef __X11_CLIENT_H_
#define __X11_CLIENT_H__

#include "client_backend.h"
#include "widget_backend.h"

#include "x11_server_widget.h"
#include "rect.h"
#include "metrics.h"

#include <X11/Xlib.h>

#include <map>


class X11ContainerWidget;
class X11ClientWidget;
class Icon;


//FIXME TODO - set border width via XConfigureWindow

class X11Client final : public WidgetBackend, public ClientBackend, public X11ServerWidget::EventHandler
{
public:
    static void init();
    static void shutdown();
    static bool handleEvent(const XEvent &ev);
    static void setFocus(X11Client *client);

    virtual ~X11Client();

    // WidgetBackend implementation
    virtual void setRect(const Rect &rect) override;
    virtual void setMapped(bool mapped) override;
    virtual void reparent(WidgetBackend *parent) override;
    virtual void redraw() override { drawFrame(); }
    virtual int maxTextHeight() const override;
    virtual void setFrontend(Widget *frontend) {
        _frontend = frontend;
    }
    virtual ClientBackend *clientBackend() override { return this; }

    // ClientBackend implementation
    virtual void setEventHandler(ClientBackend::EventHandler *handler) {
        _event_handler = handler;
    }
    virtual const Rect &rect() { return _frame->rect(); }
    virtual bool hasFocus() override { return _has_focus; }
    virtual void setFocus() override {
        setFocus(this);
    }
    virtual Icon *icon() override { return _icon; }
    virtual void requestClose() override;
    virtual void raise() override;
    virtual const std::string &name() override { return _name; }


    // X11ServerWidget::EventHandler implementation
    virtual void handleExpose() override;
    virtual void handleButtonPress(const XButtonEvent &ev) override;

    bool isMapped() { return _is_mapped; }
    void map();
    void unmap();

private:
    class CriticalSection;

    enum WindowType { NORMAL, DIALOG, OVERRIDE_REDIRECT };
    // values for the WM_STATE property
    enum WmState { STATE_WITHDRAWN = 0, STATE_NORMAL = 1, STATE_ICONIC = 3 };

    static const int _inner_frame_width = Metrics::CLIENT_INNER_FRAME_MARGIN; //FIXME remove

    static X11Client *find(Window wid);
    static X11Client *findByFrame(Window wid);
    static void create(Window wid);
    static Atom getAtomProperty(Window wid, Atom property);
    static void cancelDrag();
    static void finishDrag();

    X11Client();

//     bool validate();
    void setMappedInt(bool mapped);
    void mapInt();
    void unmapInt();
    bool refreshMapState();
    void refreshSizeHints();
    void refreshName();
    void refreshIconName();
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
    void startDrag(int x, int y);
    void makeActive();

    // TODO - use hash
    static std::map<Window, X11Client*> _wid_index;
    static std::map<Window, X11Client*> _frame_wid_index;
    static X11Client *_dragged;
    static int _dragged_original_x, _dragged_original_y;
    static int _drag_start_x, _drag_start_y;

    X11ClientWidget *_widget;
    X11ServerWidget *_frame;
    Icon *_icon;
    WindowType _window_type;
    bool _is_modal;

    bool _is_mapped = false;
    bool _has_focus = false;
    std::string _name = "<no name>";
    std::string _class;
    std::string _icon_name;
    std::string _title;
    int _min_width = 0, _min_height = 0;
    int _max_width = 0, _max_height = 0;

    Widget *_frontend = 0;
    ClientBackend::EventHandler *_event_handler = 0;
};

#endif // __X11_CLIENT_H__
