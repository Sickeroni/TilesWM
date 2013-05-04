#include "x11_client.h"

#include "x11_client_container.h"
#include "x11_client_widget.h"
#include "x11_server_widget.h"
#include "x11_application.h"
#include "x11_container_container.h"
#include "x11_canvas.h"

#include <X11/Xutil.h>
#include <X11/Xatom.h>

#include <iostream>
#include <string.h>


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
        XSync(X11Application::display(), false);
        saved_error_handler = XSetErrorHandler(&errorHandler);
    }
    in_critical_section++;
}

X11Client::CriticalSection::~CriticalSection()
{
    in_critical_section--;
    if(!in_critical_section) {
        XSync(X11Application::display(), false);
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
Atom X11Client::_net_wm_window_type = None;
Atom X11Client::_net_wm_window_type_dialog = None;


X11Client::X11Client() : Client(false),
    _widget(0), _frame(0),
    _max_width(0), _max_height(0),
    _is_dialog(false), _is_modal(false)
{
}

X11Client::~X11Client()
{
    std::cout<<"X11Client::~X11Client()\n";
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

void X11Client::setFocus()
{
    CriticalSection sec;

    refreshMapState();

    assert(isMapped());
    assert(static_cast<X11ClientContainer*>(container())->widget()->isMapped());
    assert(static_cast<X11ContainerContainer*>(container()->parent())->widget()->isMapped());

    if (X11Application::activeRootContainer()->widget()->isMapped())
        XSetInputFocus(X11Application::display(), _widget->wid(),
                       RevertToNone, CurrentTime);
}

void X11Client::raise()
{
    XRaiseWindow(X11Application::display(), _frame->wid());
}

void X11Client::setRect(const Rect &rect)
{
    assert(rect.w && rect.h);

    _frame->setRect(rect);

    const int frame_width = 5; //FIXME

    CriticalSection sec;

    Rect r;
    r.x = r.y = frame_width;
    r.w = rect.w - (2 * frame_width);
    r.h = rect.h - (2 * frame_width);
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

    _net_wm_window_type = XInternAtom(X11Application::display(), "_NET_WM_WINDOW_TYPE", false);
    _net_wm_window_type_dialog = XInternAtom(X11Application::display(), "_NET_WM_WINDOW_TYPE_DIALOG", false);

    Window unused = 0;
    Window *children = 0;
    unsigned int num_children = 0;

    XQueryTree(X11Application::display(), X11Application::root(),
               &unused, &unused, &children, &num_children);

    for (unsigned int i = 0; i < num_children; i++) {
        if (!X11ServerWidget::isServerWidget(children[i]))
            handleCreate(children[i]);
    }

    XFree(children);
}

void X11Client::handleCreate(Window wid)
{
    CriticalSection sec;

    assert(find(wid) == 0);

    bool is_modal = false;

//     X11Client *transient_for = 0;

    Window transient_for_wid = 0;
    if (XGetTransientForHint(X11Application::display(), wid, &transient_for_wid)) {
        if (transient_for_wid) {
            is_modal = true;
//             transient_for = find(transient_for_wid);
        }
    }

#if 0
    {
        int num_props = 0;
        Atom *props = XListProperties(X11Application::display(), wid, &num_props);

        {
            std::cout<<"properties:\n===========================================\n";
            for(int i = 0; i < num_props; i++) {
                char *atom_name = XGetAtomName(X11Application::display(), props[i]);
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
    if (XGetWindowAttributes(X11Application::display(), wid, &attr)) {
        if (!attr.override_redirect) {
            bool is_mapped = (attr.map_state != IsUnmapped);

            XSetWindowAttributes new_attr;
            memset(&new_attr, 0, sizeof(new_attr));

            new_attr.event_mask = attr.your_event_mask | PropertyChangeMask | FocusChangeMask;

            XChangeWindowAttributes(X11Application::display(), wid, CWEventMask, &new_attr);

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

            std::cout<<"is_dialog: "<<client->_is_dialog<<'\n';

            client->refreshSizeHints();

            if (is_mapped)
                client->_widget->unmap();

            client->_frame = X11ServerWidget::create(0, client, SubstructureNotifyMask | SubstructureRedirectMask);

            Rect frame_rect;
            frame_rect.set(attr.x, attr.y, attr.width + 10, attr.height + 10);

#if 0
            if (!attr.x && !attr.y && transient_for_wid) {
                // place client above transient_for
                XWindowAttributes transient_for_attr;
                if (XGetWindowAttributes(X11Application::display(), transient_for_wid, &transient_for_attr)) {
                    //FIXME
                    frame_rect.setPos(transient_for_attr.x + 100, transient_for_attr.y + 100);
                }
            }
#endif
            if (/*!attr.x && !attr.y && */(client->_is_dialog || is_modal)) {
                frame_rect.setPos(200, 200); //FIXME
            }

            client->_frame->setRect(frame_rect);

            _wid_index.insert(std::pair<Window, X11Client*>(wid, client));

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
    assert(_frame->isMapped() == is_mapped_cached);

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


    XAddToSaveSet(X11Application::display(), _widget->wid());

    _widget->reparent(_frame, 5, 5); // HACK: 5 = frame width

    if (!container() && !_is_dialog && !_is_modal)
        X11Application::activeRootContainer()->addClient(this);

    // notify container before mapping, to avoid visual glitches
    if (container())
        container()->handleClientAboutToBeMapped(this);

    _widget->map();
    _frame->map();

    _is_mapped = true;

    if (container())
        container()->handleClientMap(this);
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
    assert(_frame->isMapped());

    _frame->unmap();


    // if this was called because the client unmapped itself,
    // _widget is already unmapped
    if (_widget->isMapped())
        _widget->unmap();
    _widget->reparent(0);

    XRemoveFromSaveSet(X11Application::display(), _widget->wid());

    _is_mapped = false;

    if (container())
        container()->handleClientUnmap(this);
}

void X11Client::handleConfigureRequest(const XConfigureRequestEvent &ev)
{
    //FIXME - take window decoration into account
    //FIXME - both frame and client widget need to be configured

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

    if (container()) {
        changes.x = 0;
        changes.y = 0;

        int w, h;
        container()->getClientSize(w, h);

        if (changes.width > w)
            changes.width = w;
        if (changes.height > h)
            changes.height = h;

        //UGLY
        Rect rect(changes.x, changes.y, changes.width, changes.height);
        _widget->setRect(rect);

        XConfigureWindow(X11Application::display(), _widget->wid(), ev.value_mask, &changes);
    } else {
        // HACK HACK HACK

        Rect frame_rect = _frame->rect();
        //HACK
//         frame_rect.set(ev.x, ev.y, ev.width + 10, ev.height + 10);

        //HACK
        frame_rect.setSize(ev.width + 10, ev.height + 10);

        //HACK
        XWindowAttributes frame_attr;
        if (XGetWindowAttributes(X11Application::display(), _frame->wid(), &frame_attr)) {
            if (frame_attr.x || frame_attr.y)
                frame_rect.setPos(frame_attr.x, frame_attr.y);
        }

        //HACK
        changes.x = 5;
        changes.y = 5;


        //UGLY
        Rect rect(changes.x, changes.y, changes.width, changes.height);
        _widget->setRect(rect);
        //FIXME - translate coordinates
        XConfigureWindow(X11Application::display(), _widget->wid(), ev.value_mask, &changes);

        _frame->setRect(frame_rect);
    }
}

void X11Client::refreshSizeHints()
{
    std::cout<<"X11Client::refreshSizeHints()\n";

    CriticalSection sec;

    XSizeHints size_hints;
    long supplied_fields;
    if (XGetWMNormalHints(X11Application::display(), _widget->wid(), &size_hints, &supplied_fields)) {
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
    if (XGetWMName(X11Application::display(), _widget->wid(), &prop)) {
        char **list = 0;
        int count = 0;

        XmbTextPropertyToTextList(X11Application::display(), &prop,
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

    if (XGetClassHint(X11Application::display(), _widget->wid(), &class_hint)) {
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
    XGetInputFocus(X11Application::display(), &focus_return, &revert_to_return);

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
        XSetInputFocus(X11Application::display(), X11Application::root(),
                       RevertToNone, CurrentTime);

    if (focus_changed && container())
        container()->handleClientFocusChange(this);
    else if (focus_changed)
        drawFrame();
}

void X11Client::refreshWindowType()
{
    std::cout<<"X11Client::refreshWindowType()\n";

    CriticalSection sec;

    _is_dialog = false;

    Atom actual_type_return;
    int actual_format_return;
    unsigned long nitems_return;
    unsigned long bytes_after_return;
    Atom *prop_return;

    long max_length = 1024; //FIXME this is bullshit

    if (XGetWindowProperty(X11Application::display(), _widget->wid(), _net_wm_window_type,
                            0, max_length,
                            false,
                            XA_ATOM,
                            &actual_type_return,
                            &actual_format_return,
                            &nitems_return,
                            &bytes_after_return,
                            (unsigned char**)&prop_return) == Success)
    {
        if (actual_type_return == XA_ATOM) {
            Atom value = *prop_return;

//                     char *atom_name = XGetAtomName(X11Application::display(), value);
//                     if (atom_name) {
//                         std::cout<<"_net_wm_window_type: "<<atom_name<<'\n';
//                         XFree(atom_name);
//                     }

            if (value == _net_wm_window_type_dialog)
                _is_dialog = true;
        }

        XFree(prop_return);
    } else
        std::cout<<"failed to get _net_wm_window_type property.\n";
}

void X11Client::handleExpose()
{
    if (!container())
        drawFrame();
}

void X11Client::drawFrame()
{
    Canvas *canvas = _frame->canvas();

    Rect frame_rect = _frame->rect();
    frame_rect.setPos(0, 0);

    //FIXME
    canvas->erase(frame_rect);

    frame_rect.set(frame_rect.x+2, frame_rect.y+2, frame_rect.w-4, frame_rect.h-4);
    if (_has_focus)
        canvas->drawFrame(frame_rect, 0xFF8888);
}

bool X11Client::handleEvent(const XEvent &ev)
{
    //FIXME handle transient_for change

    bool handled = false;

    Window wid = 0;

    switch (ev.type) {
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
                _wid_index.erase(wid);
                if (client->container())
                    client->container()->removeClient(client);
                delete client;
                client = 0;
                break;
            case UnmapNotify:
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
                    if (ev.xproperty.atom ==  _net_wm_window_type) {
                        bool was_dialog = client->_is_dialog;
                        std::cout<<"was_dialog: "<<was_dialog<<'\n';
                        client->refreshWindowType();
                        std::cout<<"is_dialog: "<<(client->_is_dialog)<<'\n';
                        if (was_dialog != client->_is_dialog) {
                            if (client->isMapped())
                                abort(); //FIXME
                            if (client->_is_dialog) {
                                if (client->container()) {
                                    client->container()->removeClient(client);
                                    //FIXME TODO place frame
                                }
                            } else {
                                //FIXME insert into active container or something
                                abort();
                            }

                        }
                    }
                }
                break;
            }

            handled = true;
        } else if (ev.type == CreateNotify) {
            handleCreate(ev.xcreatewindow.window);
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
