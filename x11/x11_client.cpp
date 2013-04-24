#include "x11_client.h"

#include "x11_client_container.h"
#include "x11_client_widget.h"
#include "x11_server_widget.h"
#include "x11_application.h"
#include "x11_container_container.h"

#include <X11/Xutil.h>

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

std::map<Window, X11Client*> X11Client::_wid_index;


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
        XSetErrorHandler(saved_error_handler);
        X11Application::self()->ungrabServer();
    }
}

int X11Client::CriticalSection::errorHandler(Display *display, XErrorEvent *ev)
{
    if (ev->error_code != BadWindow) {
        std::cerr<<"X11Client::CriticalSection::errorHandler() - code: " <<
            static_cast<unsigned int>(ev->error_code)<<'\n';
        abort();
    } else
        return 0;
}


X11Client::X11Client() :
    _widget(0), _frame(0),
    _max_width(0), _max_height(0)
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

bool X11Client::isMapped()
{
    return _frame->isMapped();
}

void X11Client::setRect(const Rect &rect)
{
    _frame->setRect(rect);

    const int frame_width = 5;

    if (_widget) {
        CriticalSection sec;

//         int width_inc = 0, height_inc = 0;

        // get size hints
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

//             if (supplied_fields & PResizeInc) {
//                 width_inc = size_hints.width_inc;
//                 height_inc = size_hints.height_inc;
//             }
        }

        Rect r;
        r.x = r.y = frame_width;
        r.w = rect.w - (2 * frame_width);
        r.h = rect.h - (2 * frame_width);
        if (_max_width && r.w > _max_width)
            r.w = _max_width;
        if (_max_height && r.h > _max_height)
            r.h = _max_height;
        _widget->setRect(r);
    }
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

    XWindowAttributes attr;
    if (XGetWindowAttributes(X11Application::display(), wid, &attr)) {
        if (!attr.override_redirect) { // dont't manage popups etc. //FIXME - else warning on client destroy



    //         XSetWindowAttributes new__attr;
    //         memset(&new_attr, 0, sizeof(XSetWindowAttributes));
    //
    //         new_attr.event_mask = attr.your_event_mask | StructureNotifyMask;
    //
    //     //FIXME reset all attributes ?
    //
    //     XChangeWindowAttributes(_display, _root, CWEventMask, &new_root_attr);


            X11Client *client = new X11Client();

            XTextProperty prop;
            if (XGetWMName(X11Application::display(), wid, &prop)) {
                char **list = 0;
                int count = 0;

                XmbTextPropertyToTextList(X11Application::display(), &prop,
                                          &list, &count);
                if (count) {
                    client->_name = list[0];
                    XFreeStringList(list);
                }
            }

            XClassHint class_hint;
            class_hint.res_name = 0;
            class_hint.res_class = 0;

            if (XGetClassHint(X11Application::display(), wid, &class_hint)) {
                client->_name += " - ";
                client->_name += class_hint.res_name;
//                 client->_name += class_hint.res_class;

                XFree(class_hint.res_name);
                XFree(class_hint.res_class);
                class_hint.res_name = class_hint.res_class = 0;
            }

            std::cout<<"-------------------------------------------------------------------\n";
            std::cout<<"new client :"<<(client->_name)<<'\n';
            std::cout<<"-------------------------------------------------------------------\n";

            // get size hints
            XSizeHints size_hints;
            long supplied_fields;
            if (XGetWMNormalHints(X11Application::display(), wid, &size_hints, &supplied_fields)) {
                if (supplied_fields & PMaxSize) {
                    client->_max_width = size_hints.max_width;
                    client->_max_height = size_hints.max_height;
                }
                std::cout<<"has base size: "<<((supplied_fields & PBaseSize) != 0)<<'\n';
                std::cout<<"size_hints.base_width: "<<size_hints.base_width<<'\n';
                std::cout<<"size_hints.base_height: "<<size_hints.base_height<<'\n';
                std::cout<<"has resize inc: "<<((supplied_fields & PResizeInc) != 0)<<'\n';
                std::cout<<"size_hints.width_inc: "<<size_hints.width_inc<<'\n';
                std::cout<<"size_hints.height_inc: "<<size_hints.height_inc<<'\n';
            }

            client->_widget = new X11ClientWidget(wid, client);

            //FIXME - handle case: client is mapped
            //1. unmap
            //2. add to container
            //3. map

            bool is_mapped = client->_widget->isMapped();

            if (is_mapped)
                client->_widget->unmap();

            client->_frame = X11ServerWidget::create(0);

            X11Application::activeRootContainer()->addClient(client);

            if (is_mapped)
                client->map();

            _wid_index.insert(std::pair<Window, X11Client*>(wid, client));

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

//FIXME TODO abort on forbidden transition

bool X11Client::refreshMapState()
{
    std::cout<<"X11Client::handleUnmap()\n";

    CriticalSection sec;

    bool is_mapped_cached = _widget->isMapped();

    if (_widget->refreshMapState()) {

        if (is_mapped_cached == _widget->isMapped()) // state 1 or 2
            return true;
        else if (is_mapped_cached && !_widget->isMapped()) { // state 3
            //FIXME client has unmapped itself - handle
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

    if (!refreshMapState())
        return;

    if (!_frame->isMapped())
        mapInt();
}

void X11Client::mapInt()
{
    assert(!_frame->isMapped());

    {
        CriticalSection sec;

        XAddToSaveSet(X11Application::display(), _widget->wid());

        _widget->reparent(_frame);
        _widget->move(5, 5); //FIXME
        _widget->map();

        //FIXME - layout container before mapping - use _is_mapped member and set to true before layout

        _frame->map();

        if (container())
            container()->layout();
    }
}

void X11Client::unmap()
{
    std::cout<<"X11Client::unmap()\n";

    refreshMapState();

    if(_frame->isMapped())
        unmapInt();
}

void X11Client::unmapInt()
{
    assert(_frame->isMapped());

    _frame->unmap();

    CriticalSection sec;

    // if this was called because the client unmapped itself,
    // _widget is already unmapped
    if (_widget->isMapped())
        _widget->unmap();
    _widget->reparent(0);

    XRemoveFromSaveSet(X11Application::display(), _widget->wid());

    if (container())
        container()->layout();
}


//FIXME TODO set SubstructureRedirectMask for frame

void X11Client::handleConfigureRequest(X11Client *client, const XConfigureRequestEvent &ev)
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

    if (client->container()) {
        //FIXME - check for allowed size by container
        XConfigureWindow(X11Application::display(), client->_widget->wid(), ev.value_mask, &changes);
    } else {
        XConfigureWindow(X11Application::display(), client->_widget->wid(), ev.value_mask, &changes);
        //FIXME - frame
    }
}

bool X11Client::handleEvent(const XEvent &ev)
{
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
                break;
            case ConfigureRequest:
                handleConfigureRequest(client, ev.xconfigurerequest);
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
