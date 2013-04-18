#include "x11_client.h"

#include "x11_client_container.h"
#include "x11_client_widget.h"
#include "x11_application.h"
// #include "container_container.h"

#include <X11/Xutil.h>

#include <iostream>

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

X11Client::X11Client(X11Widget *widget) :
    _widget(widget)
{
    XTextProperty prop;
    if (XGetWMName(X11Application::display(), _widget->wid(), &prop)) {
        char **list = 0;
        int count = 0;

        XmbTextPropertyToTextList(X11Application::display(), &prop,
                                  &list, &count);
        if (count) {
            _name = list[0];
            XFreeStringList(list);
        }
    }

    XClassHint class_hint;
    class_hint.res_name = 0;
    class_hint.res_class = 0;

    if (XGetClassHint(X11Application::display(), _widget->wid(), &class_hint)) {
        _name += " - ";
        _name += class_hint.res_name;
//         _name += class_hint.res_class;

        XFree(class_hint.res_name);
        XFree(class_hint.res_class);
        class_hint.res_name = class_hint.res_class = 0;
    }
}

X11Client::~X11Client()
{
}

bool X11Client::validate()
{
    return _widget->validate();
}

bool X11Client::isMapped()
{
    return _widget->isMapped();
}

void X11Client::setRect(const Rect &rect)
{
    if (validate())
        _widget->setRect(rect);
}

void X11Client::setContainer(ClientContainer *container)
{
    std::cout << "void X11Client::setContainer(ClientContainer *container)\n";

    Client::setContainer(container);

    if (validate()) {
        X11ServerWidget *new_parent_widget = 0;
        if (container)
            new_parent_widget = static_cast<X11ClientContainer*>(container)->widget();

        std::cout << "new_parent_widget: " << new_parent_widget << '\n';
        _widget->reparent(new_parent_widget);
    }
}
