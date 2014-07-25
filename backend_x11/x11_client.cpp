#include "x11_client.h"

#include "x11_client_widget.h"
#include "x11_server_widget.h"
#include "x11_application.h"
#include "x11_graphics_system.h"
#include "x11_widget_backend.h"
#include "x11_global.h"
#include "icon.h"
#include "canvas.h"
#include "client_frontend.h"
#include "widget_frontend.h"
#include "common.h"

#include <X11/Xutil.h>
#include <X11/Xatom.h>

#include <cstring>


// FIXME implement iconic state


using namespace X11Global;


class X11Client::CriticalSection
{
    static int errorHandler(Display *display, XErrorEvent *ev);

    static int in_critical_section;
    int (*saved_error_handler)(Display*, XErrorEvent*);

public:
    CriticalSection();
    ~CriticalSection();
};


int X11Client::CriticalSection::in_critical_section = 0;


X11Client::CriticalSection::CriticalSection() :
    saved_error_handler(0)
{
    if (!in_critical_section) {
        X11Application::self()->grabServer();
        XSync(dpy(), false);
        saved_error_handler = XSetErrorHandler(&errorHandler);
    }
    in_critical_section++;
}

X11Client::CriticalSection::~CriticalSection()
{
    in_critical_section--;
    if(!in_critical_section) {
        XSync(dpy(), false);
        XSetErrorHandler(saved_error_handler);
        X11Application::self()->ungrabServer();
    }
}

int X11Client::CriticalSection::errorHandler(Display *display, XErrorEvent *ev)
{
    if (ev->error_code != BadWindow) {
        cerr << "X11Client::CriticalSection::errorHandler() - code: "
            << static_cast<unsigned int>(ev->error_code)
            << " - "
            << X11Application::errorCodeToString(ev->error_code)
            << '\n';
        abort();
    } else
        return 0;
}


std::map<Window, X11Client*> X11Client::_wid_index;

X11Client::X11Client() :
    _widget(0),
    _icon(0),
    _window_type(NORMAL),
    _is_modal(false)
{
}

X11Client::~X11Client()
{
    assert(!_client_frontend);
    assert(!_widget_frontend);
    debug;
    delete _icon;
    _icon = 0;
    delete _widget;
    _widget= 0;
}

void X11Client::setFocus(X11Client *client)
{
    CriticalSection sec;

    Window focused_wid = X11Application::root();

    Window prop[] = { None };

    if (client) {
        client->refreshMapState();

        if (client->_widget->isViewable()) {
            focused_wid = client->_widget->wid();
            prop[0] = focused_wid;
        }
    }

    XSetInputFocus(dpy(), focused_wid, RevertToNone, CurrentTime);

    XChangeProperty(dpy(),
                    X11Application::root(),
                    ATOM(_NET_ACTIVE_WINDOW),
                    XA_WINDOW,
                    32,
                    PropModeReplace,
                    reinterpret_cast<const unsigned char*>(prop),
                    sizeof(prop) / sizeof(prop[0]));
}

void X11Client::setRect(const Rect &rect)
{
    assert(!isOverrideRedirect());

    assert(rect.w && rect.h);

    CriticalSection sec;

    _widget->setRect(rect);
}

void X11Client::raise()
{
    if (_widget)
        XRaiseWindow(dpy(), _widget->wid());
}

void X11Client::reparent(WidgetBackend *new_parent)
{
    assert(!isOverrideRedirect());

    if (_widget) {
        X11ServerWidget *new_parent_widget = 0;

        if (new_parent)
            new_parent_widget = dynamic_cast<X11WidgetBackend*>(new_parent)->widget();

        printvar(new_parent_widget);
        _widget->reparent(new_parent_widget);
    }
}

void X11Client::grabMouseButton(int button)
{
    ModMask drag_modifier = ControlMask; //FIXME
    XGrabButton(dpy(), button, drag_modifier, _widget->wid(), true, ButtonPressMask, GrabModeAsync,
        GrabModeAsync, None, None);
}

void X11Client::init()
{
    CriticalSection sec;

    //FIXME clear these on shutdown (before closing display connection)
    Atom net_supported_values[] = {
        ATOM(_NET_WM_ICON), ATOM(_NET_WM_WINDOW_TYPE), ATOM(_NET_WM_WINDOW_TYPE_DIALOG)
    };

    const int net_supported_count = sizeof(net_supported_values) / sizeof(net_supported_values[0]);

    XChangeProperty(dpy(),
                    X11Application::root(),
                    ATOM(_NET_SUPPORTED),
                    XA_ATOM,
                    32,
                    PropModeReplace,
                    reinterpret_cast<const unsigned char*>(net_supported_values),
                    net_supported_count);

    Window unused = 0;
    Window *children = 0;
    unsigned int num_children = 0;

    XQueryTree(dpy(), X11Application::root(),
               &unused, &unused, &children, &num_children);

    for (unsigned int i = 0; i < num_children; i++) {
        if (!X11ServerWidget::isServerWidget(children[i]))
            create(children[i]);
    }

    XFree(children);
}

void X11Client::shutdown()
{
    CriticalSection sec;

    for (std::map<Window, X11Client*>::iterator it = _wid_index.begin();
            it != _wid_index.end();
            it++)
    {
        X11Client *client = it->second;
        bool was_mapped = client->isManaged(); // && _wm_state != WITHDRAWN
        if (client->_client_frontend)
            X11Application::frontend()->destroyClientFrontend(client->_client_frontend);
        client->_client_frontend = 0;
        assert(!client->_widget_frontend);
        client->_widget->reparent(0);
        // FIXME map widget depending ont the withdrawn state
        if (was_mapped)
            client->_widget->map();
        XRemoveFromSaveSet(dpy(), client->_widget->wid());
        delete client;
        client = 0;
    }

    _wid_index.clear();
}

void X11Client::create(Window wid)
{
    CriticalSection sec;

    assert(find(wid) == 0);

    bool is_modal = false;

//     X11Client *transient_for = 0;

    Window transient_for_wid = 0;
    if (XGetTransientForHint(dpy(), wid, &transient_for_wid)) {
        if (transient_for_wid) {
            is_modal = true;
//             transient_for = find(transient_for_wid);
        }
    }

#if 0
    {
        int num_props = 0;
        Atom *props = XListProperties(dpy(), wid, &num_props);

        {
            std::cout<<"properties:\n===========================================\n";
            for(int i = 0; i < num_props; i++) {
                char *atom_name = XGetAtomName(dpy(), props[i]);
                if (atom_name) {
                    std::cout<<atom_name<<'\n';
                    XFree(atom_name);
                }
            }
            std::cout<<"----------------- end properties -------------------\n";
        }

        if(props)
            XFree(props);
        props = 0;
    }
#endif

    XWindowAttributes attr;
    if (XGetWindowAttributes(dpy(), wid, &attr)) {
        if (!attr.override_redirect) {
            bool is_mapped = (attr.map_state != IsUnmapped);
            bool is_viewable = (attr.map_state == IsViewable);

            XSetWindowAttributes new_attr;
            memset(&new_attr, 0, sizeof(new_attr));

            new_attr.event_mask = attr.your_event_mask | PropertyChangeMask | FocusChangeMask;

            XChangeWindowAttributes(dpy(), wid, CWEventMask, &new_attr);

            X11Client *client = new X11Client();

            if (is_modal)
                client->_is_modal = true;

            Rect rect(attr.x, attr.y, attr.width, attr.height);
            client->_widget = new X11ClientWidget(wid, client, is_mapped, is_viewable, rect);

            client->refreshName();
            client->refreshIconName();
            client->refreshClass();
            client->refreshWindowType();

            cout<<"-------------------------------------------------------------------\n";
            cout<<"new client: "<<(client->_name)<<'\n';
            cout<<"-------------------------------------------------------------------\n";

            printvar(client->isDialog());

            client->refreshSizeHints();

#if 0
            if (!attr.x && !attr.y && transient_for_wid) {
                // place client above transient_for
                XWindowAttributes transient_for_attr;
                if (XGetWindowAttributes(dpy(), transient_for_wid, &transient_for_attr)) {
                    //FIXME
                    frame_rect.setPos(transient_for_attr.x + 100, transient_for_attr.y + 100);
                }
            }
#endif
            client->refreshIcon();

            _wid_index.insert(std::pair<Window, X11Client*>(wid, client));

            if (is_mapped)
                client->manage();
        }
    } else {
        debug << "XGetWindowAttributes() for client window" << wid << "failed.";
    }
}


/*

map state - consists of X-server state and cached state
---------------------------------

possible states:

    cached      |   X-server state
------------------------------
1.  unmapped    |   unmapped
2.  mapped      |   mapped
3.  mapped      |   unmapped
4.  unmapped    |   mapped

initial state: 1 or 2

theoretically possible transitions:

1 -> 2: (mapped by us) allowed (sync needed)
1 -> 3: (mapped by us) illegal
1 -> 4: illegal (client call is interceped)

2 -> 1: (unmapped by us) allowed (sync needed)
2 -> 3: (unmapped by client) allowed
2 -> 4: (unmapped by us) illegal

3 -> 1: allowed (cached state needs immediate update)
3 -> 2: illegal (cached state needs immediate update)
3 -> 4: illegal (client call is interceped)

4 = illegal state (unreachable)

-> allowed transitions:
1 -> 2: (mapped by us) allowed (sync needed)
2 -> 1: (unmapped by us) allowed (sync needed)
2 -> 3: (unmapped by client) allowed
3 -> 1: allowed (cached state needs immediate update)

*/

bool X11Client::refreshMapState()
{
    CriticalSection sec;

    if (_widget->refreshMapState()) {
        return true;
    } else {
        debug<<"WARNING: failed to get current map state of client"<<_name;
        return false;
    }
}

void X11Client::setMapped(bool mapped)
{
    //FIXME WM_STATE should be set here - compare with manage()
    if (_widget) {
        if (mapped)
            _widget->map();
        else
            _widget->unmap();
    }
}

void X11Client::manage()
{
    assert(!isManaged());

    CriticalSection sec;

    if (!refreshMapState())
        return;

    if (isOverrideRedirect()) {
        _widget->map();
//         setFocus(); //FIXME focus model
    } else {
        XAddToSaveSet(dpy(), _widget->wid());

        printvar((isDialog() || _is_modal));
        _client_frontend = X11Application::frontend()->createClientFrontend(this);

        const uint32_t state[2] = {
            STATE_NORMAL, None
        };

        XChangeProperty(dpy(), _widget->wid(), ATOM(WM_STATE), ATOM(CARD32), 32, PropModeReplace,
                        reinterpret_cast<const unsigned char*>(state), 2);

        if (_client_frontend)
            _client_frontend->handleMap();
        else
            _widget->map();


        X11Application::frontend()->focusActiveClient();
    }
}

void X11Client::handleUnmap()
{
    CriticalSection sec;

    if (!refreshMapState())
        return;

    if (_widget->isMapped())
        return;

    if (isOverrideRedirect()) {
        _widget->unmap();
    } else {
        if (_client_frontend)
            X11Application::frontend()->destroyClientFrontend(_client_frontend);
        _client_frontend = 0;
        assert(!_widget_frontend);

        _widget->reparent(0);

        XDeleteProperty(dpy(), _widget->wid(), ATOM(WM_STATE));

        XRemoveFromSaveSet(dpy(), _widget->wid());
    }

    X11Application::frontend()->focusActiveClient();
}

void X11Client::handleConfigureRequest(const XConfigureRequestEvent &ev)
{
    debug;

    XWindowChanges changes;
    memset(&changes, 0, sizeof(changes));
    changes.x = ev.x;
    changes.y = ev.y;
    changes.width = ev.width;
    changes.height = ev.height;
    changes.border_width = ev.border_width;
    changes.sibling = ev.above;
    changes.stack_mode = ev.detail;

    CriticalSection sec;

    if (!refreshMapState())
        return;

    if (isOverrideRedirect())
        _widget->configure(ev.value_mask, changes);
    else {
        if (ev.value_mask & CWBorderWidth)
            _widget->configure(CWBorderWidth, changes);

        if (ev.value_mask & (CWX | CWY | CWWidth | CWHeight)) {
            Rect client_rect = _widget->rect();

            if (ev.value_mask & CWX)
                client_rect.x = changes.x;
            if (ev.value_mask & CWY)
                client_rect.y = changes.y;
            if (ev.value_mask & CWWidth)
                client_rect.w = changes.width;
            if (ev.value_mask & CWHeight)
                client_rect.h = changes.height;

            limitClientRect(client_rect);

            if (_client_frontend)
                _client_frontend->handleConfigureRequest(client_rect);
            else
                _widget->setRect(client_rect);
        }
    }
}

void X11Client::refreshSizeHints()
{
    debug;

    CriticalSection sec;

    XSizeHints size_hints;
    long supplied_fields;
    if (XGetWMNormalHints(dpy(), _widget->wid(), &size_hints, &supplied_fields)) {
#if 0
            std::cout<<"size_hints.min_width: "<<size_hints.min_width<<'\n';
            std::cout<<"size_hints.min_height: "<<size_hints.min_height<<'\n';
            std::cout<<"size_hints.max_width: "<<size_hints.max_width<<'\n';
            std::cout<<"size_hints.max_height: "<<size_hints.max_height<<'\n';
            std::cout<<"has base size: "<<((supplied_fields & PBaseSize) != 0)<<'\n';
            std::cout<<"size_hints.base_width: "<<size_hints.base_width<<'\n';
            std::cout<<"size_hints.base_height: "<<size_hints.base_height<<'\n';
            std::cout<<"has resize inc: "<<((supplied_fields & PResizeInc) != 0)<<'\n';
            std::cout<<"size_hints.width_inc: "<<size_hints.width_inc<<'\n';
            std::cout<<"size_hints.height_inc: "<<size_hints.height_inc<<'\n';
#endif
        if (supplied_fields & PMaxSize) {
            _max_width = size_hints.max_width;
            _max_height = size_hints.max_height;
        }
        if (supplied_fields & PMinSize) {
            _min_width = size_hints.min_width;
            _min_height = size_hints.min_height;
        }
    } else
        debug<<"failed to get size hints for client"<<_name;
}

void X11Client::refreshName()
{
    CriticalSection sec;

    XTextProperty prop;
    if (XGetWMName(dpy(), _widget->wid(), &prop)) {
        char **list = 0;
        int count = 0;

        XmbTextPropertyToTextList(dpy(), &prop,
                                  &list, &count);
        if (count) {
            _name = list[0];
            XFreeStringList(list);
        }

        if (prop.value)
            XFree(prop.value);
    }

    _title = _class + " - " + _name;
}

void X11Client::refreshIconName()
{
    CriticalSection sec;

    XTextProperty prop;
    if (XGetWMIconName(dpy(), _widget->wid(), &prop)) {
        char **list = 0;
        int count = 0;

        XmbTextPropertyToTextList(dpy(), &prop,
                                  &list, &count);
        if (count) {
            _icon_name = list[0];
            XFreeStringList(list);
        }

        if (prop.value)
            XFree(prop.value);
    }
}

void X11Client::refreshClass()
{
    CriticalSection sec;

    XClassHint class_hint;
    class_hint.res_name = 0;
    class_hint.res_class = 0;

    if (XGetClassHint(dpy(), _widget->wid(), &class_hint)) {
        _class = class_hint.res_name;

        XFree(class_hint.res_name);
        XFree(class_hint.res_class);
        class_hint.res_name = class_hint.res_class = 0;
    }

    _title = _class + " - " + _name;
}

void X11Client::refreshFocusState()
{
    CriticalSection sec;

    Window focus_holder;
    int revert_to;
    XGetInputFocus(dpy(), &focus_holder, &revert_to);

    bool focus_changed = false;

    if (_has_focus) {
        if (_widget->wid() != focus_holder) {
            // we lost focus
            focus_changed = true;
            _has_focus = false;
        } // else: no change
    } else {
        if (_widget->wid() == focus_holder) {
            // we got focus
            focus_changed = true;
            _has_focus = true;
        } // else: no change
    }

    if (!_has_focus && focus_changed) {
        debug<<"client"<<this<<"lost focus";
        if (focus_holder) {
            X11Client *c = find(focus_holder);
            debug<<"focus moved to client"<<c;
        }
    }

    if (!focus_holder) // we lost focus and no other window is currently focused
        XSetInputFocus(dpy(), X11Application::root(), RevertToNone, CurrentTime);

    if (focus_changed) {
        if(_client_frontend)
            _client_frontend->handleFocusChanged(_has_focus);
    }
}

Atom X11Client::getAtomProperty(Window wid, Atom property)
{
    CriticalSection sec;

    Atom value = None;

    Atom actual_type_return;
    int actual_format_return;
    unsigned long nitems_return;
    unsigned long bytes_after_return;
    Atom *prop_return;

    long max_length = 1;

    if (Success == XGetWindowProperty(dpy(), wid, property,
                            0, max_length,
                            false,
                            XA_ATOM,
                            &actual_type_return,
                            &actual_format_return,
                            &nitems_return,
                            &bytes_after_return,
                            (unsigned char**)&prop_return))
    {
        if (actual_type_return == XA_ATOM)
            value = *prop_return;

        XFree(prop_return);
    }

    return value;
}

void X11Client::refreshWindowType()
{
    debug;

    CriticalSection sec;

    Atom window_type = getAtomProperty(_widget->wid(), ATOM(_NET_WM_WINDOW_TYPE));
    if (window_type != None) {
#if 1
        char *atom_name = XGetAtomName(dpy(), window_type);
        if (atom_name) {
            debug<<"window type:"<<atom_name;
            XFree(atom_name);
        }
#endif

        if (window_type == ATOM(_KDE_NET_WM_WINDOW_TYPE_OVERRIDE))
            _window_type = OVERRIDE_REDIRECT;
        else if (window_type == ATOM(_NET_WM_WINDOW_TYPE_DIALOG))
            _window_type = DIALOG;
        else
            _window_type = NORMAL;
    }
}

void X11Client::refreshIcon()
{
    debug<<"looking for _NET_WM_ICON property ...";

    CriticalSection sec;

    static const long max_size = 24*24;
    unsigned long nitems, bytesafter;
    unsigned char *ret;
    int format;
    Atom type;

    if (Success == XGetWindowProperty(dpy(),
                            _widget->wid(),
                            ATOM(_NET_WM_ICON),
                            0, max_size,
                            false, XA_CARDINAL,
                            &type,
                            &format,
                            &nitems,
                            &bytesafter,
                            &ret))
    {
        if (XA_CARDINAL == type) {
            if (nitems < 2)
                debug<<"bad length for _NET_WM_ICON property.";
            else {
                unsigned long width = 0;
                unsigned long height = 0;

                width = reinterpret_cast<unsigned long*>(ret)[0];
                height = reinterpret_cast<unsigned long*>(ret)[1];

                printvar(width);
                printvar(height);

                unsigned long size = width * height;

                if ((nitems - 2) < size)
                    debug<<"bad size for _NET_WM_ICON property.";
                else {
                    delete _icon;
                    _icon = X11Application::graphicsSystem()->createIcon(
                        width,
                        height,
                        _widget->wid(),
                        reinterpret_cast<unsigned long*>(ret) + 2,
                        0x999999 /*FIXME - taken form X11ServerWidget */);
                }
            }
        } else
            debug<<"bad data type for _NET_WM_ICON property.";

        XFree(ret);
    }
#if 0
    else if (XWMHints *hints = XGetWMHints(dpy(), _widget->wid())) {
        if (hints->flags & IconPixmapHint) {
            std::cout<<"IconPixmapHint\n";
            abort();
        }
        XFree(hints);
    }
#endif
    else
        debug<<"no _NET_WM_ICON property.";
}

int X11Client::maxTextHeight() const
{
    return 0;
}

void X11Client::requestClose()
{
    CriticalSection sec;

    XEvent xev;
    memset(&xev, 0, sizeof(xev));

    xev.type = ClientMessage;

    XClientMessageEvent &ev = xev.xclient;

    ev.type = ClientMessage;
    ev.window = _widget->wid();
    ev.message_type = ATOM(WM_PROTOCOLS);
    ev.format = 32;
    ev.data.l[0] = ATOM(WM_DELETE_WINDOW);

    XSendEvent(dpy(), _widget->wid(), false, NoEventMask, &xev);
}

void X11Client::limitClientRect(Rect &rect)
{
    if (rect.w < MIN_WIDTH)
        rect.w = MIN_WIDTH;
    if (rect.h < MIN_HEIGHT)
        rect.h = MIN_HEIGHT;
    if (rect.w > MAX_WIDTH)
        rect.w = MAX_WIDTH;
    if (rect.h > MAX_HEIGHT)
        rect.h = MAX_HEIGHT;
}

bool X11Client::handleEvent(const XEvent &ev)
{
    //FIXME handle transient_for change

    bool handled = false;

    Window wid = 0;

    switch (ev.type) {
    case ButtonPress:
        wid = ev.xbutton.window;
        break;
    case CreateNotify:
        wid = ev.xcreatewindow.window;
        break;
    case DestroyNotify:
        wid = ev.xdestroywindow.window;
        break;
    case UnmapNotify:
        wid = ev.xunmap.window;
        break;
    case MapRequest:
        wid = ev.xmaprequest.window;
        break;
    case ConfigureRequest:
        wid = ev.xconfigurerequest.window;
        break;
    default:
        wid = ev.xany.window;
    }

    if (wid) {
        X11Client *client = find(wid);

        if (client) {
            assert(client->_widget->wid() == wid);

            switch(ev.type) {
            case CreateNotify:
                abort(); // BAD - CreateNotify for already existing client
            case DestroyNotify:
                debug<<"DestroyNotify";
                _wid_index.erase(wid);
                delete client->_widget;
                client->_widget = 0;
                if (client->_client_frontend)
                    X11Application::frontend()->destroyClientFrontend(client->_client_frontend);
                client->_client_frontend = 0;
                assert(!client->_widget_frontend);
                delete client;
                client = 0;
                X11Application::self()->frontend()->focusActiveClient();
                break;
            case UnmapNotify:
                client->handleUnmap();
                break;
            case MapRequest:
                client->manage();
                break;
            case ConfigureRequest:
                client->handleConfigureRequest(ev.xconfigurerequest);
                break;
            case FocusIn:
            case FocusOut:
                client->refreshFocusState();
                break;
            case ButtonPress:
                if (client->_widget_frontend)
                    client->_widget_frontend->handleButtonPress(ev.xbutton.x_root, ev.xbutton.y_root, ev.xbutton.button);
                break;
#if 1
            case PropertyNotify:
                ClientBackend::Property prop = PROP_NONE;
                switch (ev.xproperty.atom) {
                case XA_WM_NORMAL_HINTS:
                    client->refreshSizeHints();
                    prop = PROP_SIZE_HINTS;
                    break;
                case XA_WM_NAME:
                    client->refreshName();
                    prop = PROP_NAME;
                    break;
                case XA_WM_ICON_NAME:
                    client->refreshIconName();
                    prop = PROP_ICON_NAME;
                    break;
                case XA_WM_CLASS:
                    client->refreshClass();
                    prop = PROP_CLASS;
                    break;
                case XA_WM_TRANSIENT_FOR:
                    //FIXME
                    break;
                default:
                    if (ev.xproperty.atom == ATOM(_NET_WM_WINDOW_TYPE)) {
                        //FIXME handle override_redirect
                        bool was_dialog = client->isDialog();
                        printvar(was_dialog);
                        client->refreshWindowType();
                        printvar(client->isDialog());
                        if (was_dialog != client->isDialog()) {
                            // ignore changes for managed clients - bad luck for them
                            if (!client->isManaged()) {
#if 0
                                if (client->isDialog()) {
                                    if (client->container()) {
                                        static_cast<X11ClientContainer*>(client->container())->removeClient(client);
                                        //FIXME TODO place frame
                                    }
                                } else {
                                    //FIXME insert into active container or something
                                    abort();
                                }
#endif
                            } else
                                debug<<"WARNING: client changed _NET_WM_WINDOW_TYPE while mapped.";
                        }
                    } else if (ev.xproperty.atom == ATOM(_NET_WM_ICON)) {
                        client->refreshIcon();
                        prop = PROP_ICON;
                    }
                }
                if (prop != PROP_NONE && client->_client_frontend)
                    client->_client_frontend->handlePropertyChanged(prop);
#endif
                break;
            }

            handled = true;
        } else if (ev.type == CreateNotify) {
            create(ev.xcreatewindow.window);
            handled = true;
        } else {
//             std::cout<<"no client with wid "<<wid<<'\n';
        }
    }

    return handled;
}

X11Client *X11Client::find(Window wid)
{
    std::map<Window, X11Client*>::iterator it = _wid_index.find(wid);
    if (it != _wid_index.end()) {
        return it->second;
    } else
        return 0;
}
