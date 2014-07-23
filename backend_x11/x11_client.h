#ifndef __X11_CLIENT_H_
#define __X11_CLIENT_H__

#include "client_backend.h"
#include "widget_backend.h"

#include "x11_server_widget.h"
#include "x11_client_widget.h"
#include "rect.h"
#include "metrics.h"

#include <X11/Xlib.h>

#include <map>


class X11ContainerWidget;
class Icon;
class ClientFrontend;


//FIXME TODO - set border width via XConfigureWindow

class X11Client final : public WidgetBackend, public ClientBackend
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
    virtual void redraw() override {}
    virtual int maxTextHeight() const override;
    virtual void setFrontend(WidgetFrontend *frontend) override {
        assert(!_widget_frontend || !frontend);
        _widget_frontend = frontend;
    }
    virtual void grabMouse() override {
        assert(0);
    }
    virtual void releaseMouse() override {
        assert(0);
    }
    virtual void raise() override;

    // ClientBackend implementation
    virtual WidgetBackend *widget() override { return this; }
    virtual const Rect &rect() const override { return _widget->rect(); }
    virtual bool hasFocus() const override { return _has_focus; }
    virtual void setFocus() override {
        setFocus(this);
    }
    virtual Icon *icon() const override { return _icon; }
    virtual void requestClose() override;
    virtual void grabMouseButton(int button) override;
    virtual const std::string &name() const override { return _name; }
    virtual const std::string &className() const override { return _class; }
    virtual const std::string &iconName() const override { return _icon_name; }
    virtual const std::string &title() const override { return _title; }
    virtual int minWidth() const override { return _min_width; }
    virtual int minHeight() const override { return _min_height; }
    virtual int maxWidth() const override { return _max_width; }
    virtual int maxHeight() const override { return _max_height; }

private:
    class CriticalSection;

    enum WindowType { NORMAL, DIALOG, OVERRIDE_REDIRECT };
    // values for the WM_STATE property
    enum WmState { STATE_WITHDRAWN = 0, STATE_NORMAL = 1, STATE_ICONIC = 3 };

    enum {
        MIN_WIDTH = 100,
        MIN_HEIGHT = 100,
        //FIXME - use screen size
        MAX_WIDTH = 2000,
        MAX_HEIGHT = 2000
    };

    static X11Client *find(Window wid);
    static void create(Window wid);
    static Atom getAtomProperty(Window wid, Atom property);
    static void limitClientRect(Rect &rect);

    X11Client();

    bool isManaged() {
        return _client_frontend != 0;
    }
    void manage();
    void resize(int w, int h);
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
    void handleUnmap();
    bool isOverrideRedirect() {
        return _window_type == OVERRIDE_REDIRECT;
    }
    bool isDialog() {
        return _window_type == DIALOG;
    }

    // TODO - use hash
    static std::map<Window, X11Client*> _wid_index;

    X11ClientWidget *_widget;
    Icon *_icon;
    WindowType _window_type;
    bool _is_modal;

    bool _has_focus = false;
    std::string _name = "<no name>";
    std::string _class;
    std::string _icon_name;
    std::string _title;
    int _min_width = 0, _min_height = 0;
    int _max_width = 0, _max_height = 0;

    WidgetFrontend *_widget_frontend = 0;
    ClientFrontend *_client_frontend = 0;
};

#endif // __X11_CLIENT_H__
