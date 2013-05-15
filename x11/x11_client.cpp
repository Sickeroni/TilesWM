#include "x11_client.h"

#include "x11_client_container.h"
#include "x11_client_widget.h"
#include "x11_server_widget.h"
#include "x11_application.h"
#include "x11_container_container.h"
#include "x11_canvas.h"
#include "x11_icon.h"
#include "x11_global.h"
#include "colors.h"

#include <X11/Xutil.h>
#include <X11/Xatom.h>

#include <iostream>
#include <string.h>


// FIXME implement iconic state


using namespace X11Global;
using std::cout;


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
        std::cerr << "X11Client::CriticalSection::errorHandler() - code: "
            << static_cast<unsigned int>(ev->error_code)
            << " - "
            << X11Application::errorCodeToString(ev->error_code)
            << '\n';
        abort();
    } else
        return 0;
}


std::map<Window, X11Client*> X11Client::_wid_index;
std::map<Window, X11Client*> X11Client::_frame_wid_index;
X11Client *X11Client::_dragged = 0;
int X11Client::_dragged_original_x = 0;
int X11Client::_dragged_original_y = 0;
int X11Client::_drag_start_x = 0;
int X11Client::_drag_start_y = 0;


X11Client::X11Client() : Client(false),
    _widget(0),
    _frame(0),
    _icon(0),
    _max_width(0),
    _max_height(0),
    _window_type(NORMAL),
    _is_modal(false)
{
}

X11Client::~X11Client()
{
    std::cout<<"X11Client::~X11Client()\n";
    delete _icon;
    _icon = 0;
    delete _widget;
    _widget= 0;
    delete _frame;
    _frame = 0;
}

// bool X11Client::validate()
// {
//     bool ret = _widget->validate();
//     std::cout<<"X11Client::validate(): "<<ret<<'\n';
//     return ret;
// }

Icon *X11Client::icon()
{
    return _icon;
}

void X11Client::setFocus()
{
    CriticalSection sec;

    refreshMapState();

    assert(isMapped());
    assert(isFloating() || static_cast<X11ClientContainer*>(container())->widget()->isMapped());
    assert(isFloating() || static_cast<X11ContainerContainer*>(container()->parent())->widget()->isMapped());

    if (isFloating() || X11Application::activeRootContainer()->widget()->isMapped())
        XSetInputFocus(dpy(), _widget->wid(),
                       RevertToNone, CurrentTime);
}

void X11Client::raise()
{
    assert(!isOverrideRedirect());

    XRaiseWindow(dpy(), _frame->wid());
}

void X11Client::setRect(const Rect &rect)
{
    //FIXME handle floating

    assert(!isOverrideRedirect());

    assert(rect.w && rect.h);

    _frame->setRect(rect);

    CriticalSection sec;

    Rect r;
    r.x = r.y = _inner_frame_width;
    r.w = rect.w - (2 * _inner_frame_width);
    r.h = rect.h - (2 * _inner_frame_width);
    if (_max_width && r.w > _max_width)
        r.w = _max_width;
    if (_max_height && r.h > _max_height)
        r.h = _max_height;
    if (r.w < 10)
        r.w = 10;
    if (r.h < 10)
        r.h = 10;

    _widget->setRect(r);
}

void X11Client::setContainer(ClientContainer *container)
{
    std::cout << "void X11Client::setContainer(ClientContainer *container)\n";

    assert(!isOverrideRedirect());

    Client::setContainer(container);

    X11ServerWidget *new_parent_widget = 0;
    if (container)
        new_parent_widget = static_cast<X11ClientContainer*>(container)->widget();

    std::cout << "new_parent_widget: " << new_parent_widget << '\n';
    _frame->reparent(new_parent_widget);
}


void X11Client::init()
{
    CriticalSection sec;

    XGrabButton(dpy(), 1, Mod1Mask, X11Application::root(), true, ButtonPressMask, GrabModeAsync,
            GrabModeAsync, None, None);
//     XGrabButton(dpy(), 3, Mod1Mask, X11Application::root(), true, ButtonPressMask, GrabModeAsync,
//             GrabModeAsync, None, None);


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

            XSetWindowAttributes new_attr;
            memset(&new_attr, 0, sizeof(new_attr));

            new_attr.event_mask = attr.your_event_mask | PropertyChangeMask | FocusChangeMask;

            XChangeWindowAttributes(dpy(), wid, CWEventMask, &new_attr);

            X11Client *client = new X11Client();

            if (is_modal)
                client->_is_modal = true;

            Rect rect(attr.x, attr.y, attr.width, attr.height);
            client->_widget = new X11ClientWidget(wid, client, is_mapped, rect);

            client->refreshName();
            client->refreshClass();
            client->refreshWindowType();

            std::cout<<"-------------------------------------------------------------------\n";
            std::cout<<"new client: "<<(client->_name)<<'\n';
            std::cout<<"-------------------------------------------------------------------\n";

            std::cout<<"is_dialog: "<<client->isDialog()<<'\n';

            client->refreshSizeHints();

            if (is_mapped)
                client->_widget->unmap();

            client->_frame = X11ServerWidget::create(0, Colors::CLIENT,
                                                     client,
                                                     ExposureMask | ButtonPressMask | SubstructureNotifyMask | SubstructureRedirectMask);

            Rect frame_rect;
            client->calcFrameRect(rect, frame_rect);
            if (frame_rect.y < 0) //FIXME make sure it's inside client area of screen (screen area minus panels)
                frame_rect.y = 0;
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
            if (/*!attr.x && !attr.y && */(client->isDialog()|| is_modal)) {
                frame_rect.setPos(200, 200); //FIXME
            }

            client->_frame->setRect(frame_rect);

            client->refreshIcon();
//             if (!client->_icon)
//                 client->_icon = new X11Icon(20, 20, client->_frame);

            _wid_index.insert(std::pair<Window, X11Client*>(wid, client));
            _frame_wid_index.insert(std::pair<Window, X11Client*>(client->_frame->wid(), client));

            if (is_mapped)
                client->map();
        }
    } else {
        std::cerr << "XGetWindowAttributes() for client window " << wid << "failed\n";
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
    std::cout<<"X11Client::handleUnmap()\n";

    CriticalSection sec;

    bool is_mapped_cached = _widget->isMapped();

    assert(isMapped() == is_mapped_cached);
    assert(isOverrideRedirect() || (_frame->isMapped() == is_mapped_cached));

    if (_widget->refreshMapState()) {

        if (is_mapped_cached == _widget->isMapped()) // state 1 or 2
            return true;
        else if (is_mapped_cached && !_widget->isMapped()) { // state 3
            // client has unmapped itself - handle
            unmapInt();
            return true;
        } else if (!is_mapped_cached && _widget->isMapped()) // state 4
            abort();
        else
            abort();
        return true;
    } else {
        std::cerr<<"WARNING: failed to get current map state of client \""<<_name<<"\"\n";
        return false;
    }
}

void X11Client::map()
{
    std::cout<<"X11Client::map()\n";

    CriticalSection sec;

    if (refreshMapState()) {
        if (!isMapped())
            mapInt();
    }
}

void X11Client::mapInt()
{
    assert(!isMapped());
    assert(!_frame->isMapped());

    if (isOverrideRedirect()) {
        _widget->map();
        _is_mapped = true;
        setFocus();
    } else {
        XAddToSaveSet(dpy(), _widget->wid());

        Rect client_rect;
        calcClientRect(_frame->rect(), client_rect);

        _widget->reparent(_frame, client_rect.x, client_rect.y);

        if (!container() && !isDialog() && !_is_modal)
            X11Application::activeRootContainer()->addClient(this);

        assert(isFloating() || container());

        // notify container before mapping, to avoid visual glitches
        if (container())
            container()->handleClientAboutToBeMapped(this);

        const uint32 state[2] = {
            STATE_NORMAL, None
        };

        XChangeProperty(dpy(), _widget->wid(), ATOM(WM_STATE), ATOM(CARD32), 32, PropModeReplace,
                        reinterpret_cast<const unsigned char*>(state), 2);

        _widget->map();
        _frame->map();

        _is_mapped = true;

        if (container())
            container()->handleClientMap(this);
        else
            setFocus();
    }
}

void X11Client::unmap()
{
    std::cout<<"X11Client::unmap()\n";

    CriticalSection sec;

    if (refreshMapState()) {
        if(isMapped())
            unmapInt();
    }
}

void X11Client::unmapInt()
{
    assert(isMapped());
    assert(isOverrideRedirect() || _frame->isMapped());

    if (isOverrideRedirect()) {
        // if this was called because the client unmapped itself,
        // _widget is already unmapped
        if (_widget->isMapped())
            _widget->unmap();

        _is_mapped = false;
    } else {
        _frame->unmap();

        // if this was called because the client unmapped itself,
        // _widget is already unmapped
        if (_widget->isMapped())
            _widget->unmap();
        _widget->reparent(0);

        XDeleteProperty(dpy(), _widget->wid(), ATOM(WM_STATE));

        XRemoveFromSaveSet(dpy(), _widget->wid());

        _is_mapped = false;

        if (container())
            container()->removeClient(this);
    }
}

void X11Client::handleConfigureRequest(const XConfigureRequestEvent &ev)
{
    std::cout<<"X11Client::handleConfigureRequest()\n";

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

    if (isOverrideRedirect() || !isMapped())
        _widget->configure(ev.value_mask, changes);
    else if (isMapped()) {
        if (container()) {
            // ignore geometry/stacking requests, as that should be changed on our behalf only
            if (ev.value_mask & CWBorderWidth)
                _widget->configure(CWBorderWidth, changes);
        } else if (isDialog() || _is_modal) {
            Rect client_rect = _widget->rect();
            if (ev.value_mask & CWX)
                client_rect.x = changes.x;
            if (ev.value_mask & CWY)
                client_rect.y = changes.y;
            if (ev.value_mask & CWWidth)
                client_rect.w = changes.width;
            if (ev.value_mask & CWHeight)
                client_rect.h = changes.height;

            // new frame rect based on requested client rect
            Rect frame_rect;
            calcFrameRect(client_rect, frame_rect);

            frame_rect.setPos(client_rect.x, client_rect.y);
            calcClientRect(frame_rect, client_rect);

            //FIXME use a less random placement - see also X11Client::create()
            frame_rect.setPos(200, 200);

            changes.x = client_rect.x;
            changes.y = client_rect.y;
            changes.width = client_rect.w;
            changes.height = client_rect.h;

            // ignore stacking requests
            unsigned int values = ev.value_mask & ~(CWSibling | CWStackMode);

            _widget->configure(values, changes);
            _frame->setRect(frame_rect);
        }
    }
}

void X11Client::refreshSizeHints()
{
    std::cout<<"X11Client::refreshSizeHints()\n";

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
    } else
        std::cerr<<"failed to get size hints for client \""<<_name<<"\"\n";
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
            _x11_name = list[0];
            XFreeStringList(list);
        }
    }

    _name = _x11_class + " - " + _x11_name;
}

void X11Client::refreshClass()
{
    CriticalSection sec;

    XClassHint class_hint;
    class_hint.res_name = 0;
    class_hint.res_class = 0;

    if (XGetClassHint(dpy(), _widget->wid(), &class_hint)) {
        _x11_class = class_hint.res_name;

        XFree(class_hint.res_name);
        XFree(class_hint.res_class);
        class_hint.res_name = class_hint.res_class = 0;
    }

    _name = _x11_class + " - " + _x11_name;
}

void X11Client::refreshFocusState()
{
    CriticalSection sec;

    Window focus_return;
    int revert_to_return;
    XGetInputFocus(dpy(), &focus_return, &revert_to_return);

    bool focus_changed = false;

    if (_has_focus) {
        if (_widget->wid() != focus_return) {
            // we lost focus
            focus_changed = true;
            _has_focus = false;
        } // else: no change
    } else {
        if (_widget->wid() == focus_return) {
            // we got focus
            focus_changed = true;
            _has_focus = true;
        } // else: no change
    }

    if (!focus_return)
        X11Application::activeRootContainer()->setFocus();

    if (focus_changed && container())
        container()->handleClientFocusChange(this);
//     else
    if (focus_changed)
        drawFrame();
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
    std::cout<<"X11Client::refreshWindowType()\n";

    CriticalSection sec;

    Atom window_type = getAtomProperty(_widget->wid(), ATOM(_NET_WM_WINDOW_TYPE));
    if (window_type != None) {
#if 1
        char *atom_name = XGetAtomName(dpy(), window_type);
        if (atom_name) {
            std::cout<<"window type: "<<atom_name<<'\n';
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
    std::cout<<"looking for _NET_WM_ICON property ...\n";

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
            std::cout<<"got icon: ";

            if (nitems < 2)
                std::cerr<<"bad length for _NET_WM_ICON property.\n";
            else {
                unsigned long width = 0;
                unsigned long height = 0;

                width = reinterpret_cast<unsigned long*>(ret)[0];
                height = reinterpret_cast<unsigned long*>(ret)[1];

                std::cout<<"width: "<<width<<" height: "<<height<<'\n';

                unsigned long size = width * height;

                if ((nitems - 2) < size)
                    std::cerr<<"bad length for _NET_WM_ICON property.\n";
                else {
                    delete _icon;
                    _icon = new X11Icon(width, height, _frame,
                                        reinterpret_cast<unsigned long*>(ret) + 2,
                                        0x999999 /*FIXME - taken form X11ServerWidget */);
                }
            }
        } else
            std::cerr<<"bad data type for _NET_WM_ICON property.\n";

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
        std::cout<<"no _NET_WM_ICON property.\n";
}

void X11Client::handleExpose()
{
//     if (!container())
        drawFrame();
}

void X11Client::drawFrame()
{
    Client::drawFrame(_frame->canvas());
}

int X11Client::maxTextHeight()
{
    return _frame->canvas()->maxTextHeight();
}

void X11Client::startDrag(int x, int y)
{
    //FIXME what if the pointer is already grabbed ?
    cout<<"X11Client::startDrag()\n";
    assert(!_dragged);

    _dragged = this;
    _dragged_original_x = _frame->rect().x;
    _dragged_original_y = _frame->rect().y;

    _drag_start_x = x;
    _drag_start_y = y;

    XGrabPointer(dpy(), _frame->wid(), true,
                 PointerMotionMask | ButtonReleaseMask,
                 GrabModeAsync, GrabModeAsync, None, None, CurrentTime);
}

void X11Client::cancelDrag()
{
    cout<<"X11Client::cancelDrag()\n";
    finishDrag();
}

void X11Client::finishDrag()
{
    cout<<"X11Client::finishDrag()\n";
    _dragged = 0;
    _dragged_original_x = _dragged_original_y = 0;
    _drag_start_x = _drag_start_y = 0;
    XUngrabPointer(dpy(), CurrentTime);
}

bool X11Client::handleEvent(const XEvent &ev)
{
    //FIXME handle transient_for change

    bool handled = false;

    Window wid = 0;

    switch (ev.type) {
    case MotionNotify:
//         cout<<"MotionNotify\n";
        if (_dragged) {
            XEvent motion_event = ev;
            while(XCheckTypedEvent(dpy(), MotionNotify, &motion_event)) {} //FIXME - what about motion events after botton release ?
            int xdiff = motion_event.xbutton.x_root - _drag_start_x;
            int ydiff = motion_event.xbutton.y_root - _drag_start_y;
//             cout<<"startx: "<<_drag_start_x<<" starty: "<<_drag_start_y<<'\n';
//             cout<<"xdiff: "<<xdiff<<" ydiff: "<<ydiff<<'\n';
            _dragged->_frame->move(_dragged_original_x + xdiff, _dragged_original_y + ydiff);
        }
        break;
    case ButtonPress:
        assert(!_dragged);
        if (ev.xbutton.subwindow != None) {
            X11Client *client = findByFrame(ev.xbutton.subwindow);
            if (!client)
                client = find(ev.xbutton.subwindow);
            if (client && (ev.xbutton.button == 1) && (ev.xbutton.state & Mod1Mask)) {
                assert(!_dragged);
                client->startDrag(ev.xbutton.x_root, ev.xbutton.y_root);
                return true;
            }
        } else {
            X11Client *client = findByFrame(ev.xbutton.window);
            if (client) {
                client->setFocus();
                client->raise();
                if (ev.xbutton.button == 1)
                    client->startDrag(ev.xbutton.x_root, ev.xbutton.y_root);
                return true;
            }
        }
        break;
    case ButtonRelease:
        if (ev.xbutton.button == 1 && _dragged) {
            finishDrag();
            return true;
        }
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
                if (_dragged == client)
                    cancelDrag();
                _frame_wid_index.erase(client->_frame->wid());
                _wid_index.erase(wid);
                if (client->container())
                    client->container()->removeClient(client);
                delete client;
                client = 0;
                break;
            case UnmapNotify:
                if (_dragged == client)
                    cancelDrag();
                client->refreshMapState();
                break;
            case MapRequest:
                client->map();
                if (ClientContainer *container =  client->container()) {
                    container->setActiveClient(client);
                    container->makeActive();
                    client->setFocus();
                }
                break;
            case ConfigureRequest:
                client->handleConfigureRequest(ev.xconfigurerequest);
                break;
            case FocusIn:
            case FocusOut:
                client->refreshFocusState();
                break;
            case PropertyNotify:
                switch (ev.xproperty.atom) {
                case XA_WM_NORMAL_HINTS:
                    client->refreshSizeHints();
                    if (client->container())
                        client->container()->layout();
                    break;
                case XA_WM_NAME:
                    client->refreshName();
                    if (client->container())
                        client->container()->redraw();
                    break;
                case XA_WM_CLASS:
                    client->refreshClass();
                    if (client->container())
                        client->container()->redraw();
                    break;
                case XA_WM_TRANSIENT_FOR:
                    //FIXME
                    break;
                default:
                    if (ev.xproperty.atom == ATOM(_NET_WM_WINDOW_TYPE)) {
                        //FIXME handle override_redirect
                        bool was_dialog = client->isDialog();
                        std::cout<<"was_dialog: "<<was_dialog<<'\n';
                        client->refreshWindowType();
                        std::cout<<"is_dialog: "<<(client->isDialog())<<'\n';
                        if (was_dialog != client->isDialog()) {
                            if (client->isMapped())
                                abort(); //FIXME
                            if (client->isDialog()) {
                                if (client->container()) {
                                    client->container()->removeClient(client);
                                    //FIXME TODO place frame
                                }
                            } else {
                                //FIXME insert into active container or something
                                abort();
                            }

                        }
                    } else if (ev.xproperty.atom == ATOM(_NET_WM_ICON)) {
                        client->refreshIcon();
                        if (client->container())
                            client->container()->layout(); //FIXME use handleClientIconChanged()
                    }
                }
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

X11Client *X11Client::findByFrame(Window wid)
{
    std::map<Window, X11Client*>::iterator it = _frame_wid_index.find(wid);
    if (it != _frame_wid_index.end()) {
        return it->second;
    } else
        return 0;
}
