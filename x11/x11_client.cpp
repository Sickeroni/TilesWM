#include "x11_client.h"

#include "x11_client_container.h"
#include "x11_client_widget.h"
#include "x11_server_widget.h"
#include "x11_application.h"
#include "x11_container_container.h"

#include <X11/Xutil.h>

#include <iostream>
#include <string.h>

#if 0

void X11Client::init()
{
    Window unused = 0;
    Window *children = 0;
    unsigned int num_children = 0;

    XQueryTree(X11Application::display(), X11Application::root(),
               &unused, &unused, &children, &num_children);

    for (unsigned int i = 0; i < num_children; i++) //FIXME - what if a window gets destroyed inbetween ?
        newClient(children[i]);

    XFree(children);
}


void X11Client::newClient(Window window)
{
//     printf("Client::windowCreated(): %d\n", (int)window);

    X11Widget *widget = X11Widget::find(window);

    if (widget) {
//         printf("WARNING: client already created for window %d\n", (int)window);
    } else {
        XWindowAttributes attr;
        if (XGetWindowAttributes(X11Application::display(), window, &attr)) {
            if (attr.override_redirect) // dont't manage popups etc. //FIXME - else warning on client destroy
                return;


            X11Client *c = new X11Client(window);
//             _client_from_window[window] = c;

//             Container::root()->addClient(c);
            X11Application::activeRootContainer()->addClient(c);
        } //FIXME else error
    }

//     printf("_client_from_window.size(): %d\n", (int)_client_from_window.size());
}

#endif


int newClientWidgetErrorHandler(Display *display, XErrorEvent *ev)
{
    if (ev->error_code != BadWindow) {
        std::cerr<<"X11ClientWidget::newClientWidget() caused X error - code: " <<
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

bool X11Client::validate()
{
    bool ret = _widget->validate();
    std::cout<<"X11Client::validate(): "<<ret<<'\n';
    return ret;
}

bool X11Client::isMapped()
{
    return _frame->isMapped();
}

void X11Client::setRect(const Rect &rect)
{
    _frame->setRect(rect);

    const int frame_width = 5;

    if (_widget) {
        X11Application::self()->grabServer();

        XSync(X11Application::display(), false);

        int (*error_handler)(Display *, XErrorEvent *) =
                                XSetErrorHandler(&newClientWidgetErrorHandler);

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


        XSync(X11Application::display(), false);

        XSetErrorHandler(error_handler);

        X11Application::self()->ungrabServer();
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


// void X11Client::onWidgetDestroyed()
// {
//     //FIXME delete here ?
//     // delete _widget
//     _widget = 0;
// }



#if 0
//FIXME  - change to onUnmap() - map is already intercepted
void X11Client::onMapStateChanged()
{
    if (_widget->isMapped())
        _frame->map();
//         XUnmapWindow(X11Application::display(), _frame->wid());
    else
        _frame->unmap();
//         XMapWindow(X11Application::display(), _frame->wid());

    Client::onMapStateChanged();
}
#endif

void X11Client::init()
{
    X11Application::self()->grabServer();

    Window unused = 0;
    Window *children = 0;
    unsigned int num_children = 0;

    XQueryTree(X11Application::display(), X11Application::root(),
               &unused, &unused, &children, &num_children);

    for (unsigned int i = 0; i < num_children; i++) {
        if (!X11Widget::find(children[i]))
            handleCreate(children[i]);
    }

    XFree(children);

    X11Application::self()->ungrabServer();
}

void X11Client::handleDestroy(X11Widget *widget)
{
    std::cout<<"X11Client::handleDestroy\n";
    assert(client);

    X11Client *client = static_cast<X11ClientWidget*>(widget)->client();
    if (client->container())
        client->container()->removeClient(client);

    delete client;
}

void X11Client::handleCreate(Window wid)
{
    X11Application::self()->grabServer();
    XSync(X11Application::display(), false);
    int (*error_handler)(Display *, XErrorEvent *) =
                            XSetErrorHandler(&newClientWidgetErrorHandler);

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
        }
    } else {
        std::cerr << "XGetWindowAttributes() for client window " << wid << "failed\n";
    }

    XSync(X11Application::display(), false);
    XSetErrorHandler(error_handler);
    X11Application::self()->ungrabServer();
}

void X11Client::map()
{
    X11Application::self()->grabServer();
    XSync(X11Application::display(), false);
    int (*error_handler)(Display *, XErrorEvent *) =
                            XSetErrorHandler(&newClientWidgetErrorHandler);

    XAddToSaveSet(X11Application::display(), _widget->wid());

    _widget->reparent(_frame);
    _widget->move(5, 5); //FIXME
    _widget->map();

    XSync(X11Application::display(), false);
    XSetErrorHandler(error_handler);
    X11Application::self()->ungrabServer();

    //FIXME - layout container before mapping - use _is_mapped member and set to true berfore layout

    _frame->map();

    if (container())
        container()->layout();
}

void X11Client::unmap()
{
    _frame->unmap();

    X11Application::self()->grabServer();
    XSync(X11Application::display(), false);
    int (*error_handler)(Display *, XErrorEvent *) =
                            XSetErrorHandler(&newClientWidgetErrorHandler);

    _widget->unmap();
    _widget->reparent(0);

    XRemoveFromSaveSet(X11Application::display(), _widget->wid());

    XSync(X11Application::display(), false);
    XSetErrorHandler(error_handler);
    X11Application::self()->ungrabServer();
}


void X11Client::handleMapRequest(X11Widget *widget)
{
    std::cout<<"X11Client::handleMapRequest()\n";

    X11Client *client = static_cast<X11ClientWidget*>(widget)->client();

    assert(client);

    client->map();
#if 0
    X11Application::self()->grabServer();
    XSync(X11Application::display(), false);
    int (*error_handler)(Display *, XErrorEvent *) =
                            XSetErrorHandler(&newClientWidgetErrorHandler);

    XAddToSaveSet(X11Application::display(), widget->wid());
    widget->reparent(client->_frame);
    widget->move(5, 5); //FIXME
    widget->map();

    XSync(X11Application::display(), false);
    XSetErrorHandler(error_handler);
    X11Application::self()->ungrabServer();
#endif
}

void X11Client::handleConfigureRequest(X11Widget *widget, const XConfigureRequestEvent &ev)
{
    //FIXME - take window decoration into account
    //FIXME  - both frame and client widget need to be configured

    std::cout<<"X11Client::handleConfigureRequest()\n";

    X11Client *client = static_cast<X11ClientWidget*>(widget)->client();

    assert(client);

    XWindowChanges changes;
    memset(&changes, 0, sizeof(changes));
    changes.x = ev.x;
    changes.y = ev.y;
    changes.width = ev.width;
    changes.height = ev.height;
    changes.border_width = ev.border_width;
    changes.sibling = ev.above;
    changes.stack_mode = ev.detail;

    X11Application::self()->grabServer();
    XSync(X11Application::display(), false);
    int (*error_handler)(Display *, XErrorEvent *) =
                            XSetErrorHandler(&newClientWidgetErrorHandler);

    if (client->container()) {
            //FIXME - check for allowed size by container
        XConfigureWindow(X11Application::display(), widget->wid(), ev.value_mask, &changes);
    } else {
        XConfigureWindow(X11Application::display(), widget->wid(), ev.value_mask, &changes);
        //FIXME - frame
    }

    XSync(X11Application::display(), false);
    XSetErrorHandler(error_handler);
    X11Application::self()->ungrabServer();
}
