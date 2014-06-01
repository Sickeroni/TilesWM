#include "x11_application.h"

#include "x11_container_container.h"
#include "x11_client_container.h"
#include "x11_client.h"
#include "x11_widget.h"
#include "x11_shortcut_set.h"

#include "workspace.h"
#include "monitor.h"
#include "client_container.h"
#include "config.h"
#include "common.h"

#include <sys/select.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>



#if 0
---------------------------------------------
X11 events
---------------------------------------------
#define KeyPress        2
#define KeyRelease      3
#define ButtonPress     4
#define ButtonRelease       5
#define MotionNotify        6
#define EnterNotify     7
#define LeaveNotify     8
#define FocusIn         9
#define FocusOut        10
#define KeymapNotify        11
#define Expose          12
#define GraphicsExpose      13
#define NoExpose        14
#define VisibilityNotify    15
#define CreateNotify        16
#define DestroyNotify       17
#define UnmapNotify     18
#define MapNotify       19
#define MapRequest      20
#define ReparentNotify      21
#define ConfigureNotify     22
#define ConfigureRequest    23
#define GravityNotify       24
#define ResizeRequest       25
#define CirculateNotify     26
#define CirculateRequest    27
#define PropertyNotify      28
#define SelectionClear      29
#define SelectionRequest    30
#define SelectionNotify     31
#define ColormapNotify      32
#define ClientMessage       33
#define MappingNotify       34
#define GenericEvent        35
#define LASTEvent       36  /* must be bigger than any event # */
#endif




X11Application *X11Application::_self = 0;


const char *X11Application::eventTypeToString(size_t type)
{
    static const char *table[] = {
        "unknown event id",
        "unknown event id",
        "KeyPress",
        "KeyRelease",
        "ButtonPress",
        "ButtonRelease",
        "MotionNotify",
        "EnterNotify",
        "LeaveNotify",
        "FocusIn",
        "FocusOut",
        "KeymapNotify",
        "Expose",
        "GraphicsExpose",
        "NoExpose",
        "VisibilityNotify",
        "CreateNotify",
        "DestroyNotify",
        "UnmapNotify",
        "MapNotify",
        "MapRequest",
        "ReparentNotify",
        "ConfigureNotify",
        "ConfigureRequest",
        "GravityNotify",
        "ResizeRequest",
        "CirculateNotify",
        "   CirculateRequest",
        "PropertyNotify",
        "SelectionClear",
        "SelectionRequest",
        "SelectionNotify",
        "ColormapNotify",
        "ClientMessage",
        "MappingNotify",
        "GenericEvent"
    };

    if (type >= (sizeof(table) / sizeof(table[0])))
        return "unknown event id";
    else
        return table[type];
}

const char *X11Application::errorCodeToString(size_t error_code)
{
    // core protocol errors
    static const char *core_errors[] = {
        "Success",
        "BadRequest",
        "BadValue",
        "BadWindow",
        "BadPixmap",
        "BadAtom",
        "BadCursor",
        "BadFont",
        "BadMatch",
        "BadDrawable",
        "BadAccess",
        "BadAlloc",
        "BadColor",
        "BadGC",
        "BadIDChoice",
        "BadName",
        "BadLength",
        "BadImplementation"
    };

    if (error_code >= (sizeof(core_errors) / sizeof(core_errors[0])))
        return "unknown error code";
    else
        return core_errors[error_code];
}


X11Application::X11Application() :
    _dpy(0),
    _root(0),
    _num_server_grabs(0),
    _quit_requested(false),
    _monitor(0)
{
    if (_self)
        abort();
    _self = this;
}

X11Application::~X11Application()
{
    _self = 0;
}

void X11Application::quit(int signum)
{
    printf("   Quitting ...\n");
    self()->_quit_requested = true;
}

// int xErrorHandler(Display *display, XErrorEvent *ev)
// {
//     abort();
// }

bool X11Application::init()
{
    Config::init();

    signal(SIGINT, &quit);

//     XSetErrorHandler(&xErrorHandler);

    /* return failure status if we can't connect */
    if(!(_dpy = XOpenDisplay(0))) {
        cerr << "ERROR: Can't open display.\n";
        return false;
    }

    XSynchronize(_dpy, true);

    grabServer();
 
    _root = DefaultRootWindow(_dpy);

    XWindowAttributes root_attr;
    if (!XGetWindowAttributes(_dpy, _root, &root_attr)) {
        cerr << "ERROR: XGetWindowAttributes() failed for root window.\n";
        XCloseDisplay(_dpy);
        _dpy = 0;
        return false;
    }

    if (root_attr.all_event_masks & SubstructureRedirectMask) {
        cerr<<"ERROR: Another window manager is already running.\n";
        XCloseDisplay(_dpy);
        _dpy = 0;
        return false;
    }

    XSetWindowAttributes new_root_attr;
    memset(&new_root_attr, 0, sizeof(XSetWindowAttributes));

    new_root_attr.event_mask = SubstructureNotifyMask | SubstructureRedirectMask;

    XChangeWindowAttributes(_dpy, _root, CWEventMask, &new_root_attr);

    XSync(_dpy, false);

    Application::init();

    _monitor = new Monitor();
    _monitor->setSize(root_attr.width, root_attr.height);

    X11Client::init();

    // set initial focus
    focusActiveClient();

//     XFlush(_dpy);
    XSync(_dpy, false);

    ungrabServer();

    cout<<"initialisation finished.\n";

    return true;
}

void X11Application::shutdown()
{
    grabServer();

    Application::shutdown();

    //FIXME delete root container

    XSetWindowAttributes new_root_attr;
    memset(&new_root_attr, 0, sizeof(XSetWindowAttributes));

    XChangeWindowAttributes(_dpy, _root, CWEventMask, &new_root_attr); // clears event mask

    setFocus(0);

    XSync(_dpy, false);

    _atoms.clear();

    ungrabServer();

    XCloseDisplay(_dpy);
    _dpy = 0;
}

void X11Application::eventLoop()
{
    cout<<"------------------------------\n";
    cout<<"event loop started.\n";
    cout<<"------------------------------\n";

    int x11_fd = ConnectionNumber(_dpy);

    timeval timeout_spec;
    fd_set x11_fd_set;
    XEvent ev;

    for( ; ; )
    {
//         XFlush(_dpy);
        if (_quit_requested)
            return;

        while (!XPending(_dpy)) {
            // Wait for X Event or a timeout
            FD_ZERO(&x11_fd_set);
            FD_SET(x11_fd, &x11_fd_set);

            timeout_spec.tv_usec = 0;
            timeout_spec.tv_sec = 2;

            select(x11_fd + 1, &x11_fd_set, 0, 0, &timeout_spec);

            if (_quit_requested)
                return;
        }

        XNextEvent(_dpy, &ev);

        if (ev.type == KeyPress && X11Shortcut::handleKeyPress(ev.xkey)) {
            // NO-OP
        }
        else if (ev.xany.window && X11Widget::handleEvent(ev)) {
            // NO-OP
        }
        else {
//             cout<<"unhandled event: "<<eventTypeToString(ev.type)<<'\n';
        }

        XSync(_dpy, false);

//         XUngrabServer(_dpy);
    }
}

void X11Application::grabServer()
{
    assert(_num_server_grabs >= 0);

    if (!_num_server_grabs)
        XGrabServer(_dpy);

    _num_server_grabs++;
}

void X11Application::ungrabServer()
{
    _num_server_grabs--;

    assert(_num_server_grabs >= 0);

    if (!_num_server_grabs)
        XUngrabServer(_dpy);
}

Monitor *X11Application::activeMonitor()
{
    return _monitor;
}

Workspace *X11Application::activeWorkspace()
{
    return self()->activeMonitor()->workspace();
}

X11ContainerContainer *X11Application::activeRootContainer()
{
    if (self()->activeLayer() == LAYER_TILED)
        return static_cast<X11ContainerContainer*>(activeWorkspace()->rootContainer());
    else
        return 0;
}

X11ClientContainer *X11Application::activeClientContainer()
{
    return static_cast<X11ClientContainer*>(Application::activeClientContainer());
}

void X11Application::setActiveMonitor(Monitor *monitor)
{
    //FIXME
}

Workspace *X11Application::createWorkspace()
{
    Workspace *w = new Workspace();
    _workspaces.push_back(w);
    return w;
}

ContainerContainer *X11Application::createContainerContainer()
{
    return new X11ContainerContainer();
}

ClientContainer *X11Application::createClientContainer()
{
    return new X11ClientContainer();
}

ShortcutSet *X11Application::createShortcutSet(std::string name)
{
    return new X11ShortcutSet(name);
}

X11Client *X11Application::activeClient()
{
    return static_cast<X11Client*>(Application::activeClient());
}

void X11Application::setFocus(Client *client)
{
    if (! (client && static_cast<X11Client*>(client)->setFocus()) )
        XSetInputFocus(dpy(), X11Application::root(), RevertToNone, CurrentTime);
}

Atom X11Application::atom(const char *name)
{
    Atom &value = _atoms[name];
    if (value == None)
        value = XInternAtom(_dpy, name, false);

    return value;
}
