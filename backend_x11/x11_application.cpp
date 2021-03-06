#include "x11_application.h"

#include "x11_client.h"
#include "x11_widget.h"
#include "x11_graphics_system.h"
#include "x11_widget_backend.h"

#include "frontend_base.h"
#include "string_util.h"
#include "config.h"
#include "common.h"

#include <X11/Xcursor/Xcursor.h>
#include <X11/cursorfont.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


X11GraphicsSystem *createX11GraphicsSystem(Display *dpy);


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
        "CirculateRequest",
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


X11Application::X11Application(FrontendBase *frontend) :
    _dpy(0),
    _root(None),
    _num_server_grabs(0),
    _quit_requested(false),
    _frontend(frontend)
{
}

X11Application::~X11Application()
{
    _self = 0;
}

void X11Application::quit(int /*signum*/)
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
    Config::self()->setDefaults();

    signal(SIGINT, &quit);

//     XSetErrorHandler(&xErrorHandler);

    /* return failure status if we can't connect */
    if(!(_dpy = XOpenDisplay(0))) {
        cerr << "ERROR: Can't open display.\n";
        return false;
    }

    XSynchronize(_dpy, false);

    grabServer();
 
    _root = DefaultRootWindow(_dpy);

    XWindowAttributes root_attr;
    if (!XGetWindowAttributes(_dpy, _root, &root_attr)) {
        cerr << "ERROR: XGetWindowAttributes() failed for root window.\n";
        _root = None;
        XCloseDisplay(_dpy);
        _dpy = 0;
        return false;
    }

    if (root_attr.all_event_masks & SubstructureRedirectMask) {
        cerr<<"ERROR: Another window manager is already running.\n";
        _root = None;
        XCloseDisplay(_dpy);
        _dpy = 0;
        return false;
    }

    _root_width = root_attr.width;
    _root_height = root_attr.height;

    XSetWindowAttributes new_root_attr;
    memset(&new_root_attr, 0, sizeof(XSetWindowAttributes));

    new_root_attr.event_mask = SubstructureNotifyMask | SubstructureRedirectMask;

    XChangeWindowAttributes(_dpy, _root, CWEventMask, &new_root_attr);

    Cursor cursor = getCursor(XC_left_ptr);
    XDefineCursor(_dpy, _root, cursor);

    XSync(_dpy, false);

    _graphics_system = createX11GraphicsSystem(_dpy);

    assert(!_self);
    _self = this;

    _frontend->init(this);

    X11Client::init();

    // set initial focus
    _frontend->focusActiveClient();

//     XFlush(_dpy);
    XSync(_dpy, false);

    ungrabServer();

    cout<<"initialisation finished.\n";

    return true;
}

void X11Application::shutdown()
{
    grabServer();
    XSync(_dpy, false);

    X11Client::shutdown();

    _frontend->shutdown();

    XSetWindowAttributes new_root_attr;
    memset(&new_root_attr, 0, sizeof(XSetWindowAttributes));

    XChangeWindowAttributes(_dpy, _root, CWEventMask, &new_root_attr); // clears event mask

    X11Client::setFocus(0);

    delete _graphics_system;
    _graphics_system = 0;

    for (auto it = _cursors.begin(); it != _cursors.end(); it++)
        XFreeCursor(_dpy, it->second);
    _cursors.clear();

    _atoms.clear();

    XSync(_dpy, false);

    _root = None;

    ungrabServer();

    XCloseDisplay(_dpy);
    _dpy = 0;

    Config::shutdown();

    _self = 0;
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

        if (ev.type == KeyPress && handleKeyPress(ev.xkey)) {
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

WidgetBackend *X11Application::createWidgetBackend()
{
    return new X11WidgetBackend();
}

Icon *X11Application::loadImage(std::string filename)
{
    printvar(filename);
    return _graphics_system->loadImage(filename);
}

Atom X11Application::atom(const char *name)
{
    Atom &value = _atoms[name];
    if (value == None)
        value = XInternAtom(_dpy, name, false);

    return value;
}

bool X11Application::handleKeyPress(const XKeyEvent &ev)
{
    debug;
    X11Global::KeySequence key_sequence(
        XLookupKeysym(const_cast<XKeyEvent*>(&ev), 0),
        ev.state & ~(LockMask | num_lock_mask));

    return _frontend->handleKeySequence(&key_sequence);
}

AbstractKeySequence *X11Application::parseKeySequence(std::string key_sequence_str)
{
    std::vector<std::string> tokens;
    StringUtil::tokenize(key_sequence_str, '+', tokens);

    X11Global::KeySequence key_sequence(NoSymbol, 0);

    for (size_t i = 0; i < tokens.size(); i++) {
        std::string &s = tokens[i];

        if (s == "CTRL")
            key_sequence.mod_mask |= ControlMask;
        else if (s == "MOD1")
            key_sequence.mod_mask |= Mod1Mask;
        else if (s == "SHIFT")
            key_sequence.mod_mask |= ShiftMask;
        else {
            key_sequence.key_sym = XStringToKeysym(s.c_str());
            if (key_sequence.key_sym == NoSymbol) {
                debug<<"invalid key symbol"<<s<<"in key sequence"<<key_sequence_str;
                return 0;
            }
            break;
        }
    }

    if (key_sequence.key_sym == NoSymbol) {
        debug<<"missing key symbol in key sequence"<<key_sequence_str;
        return 0;
    } else
        return new X11Global::KeySequence(key_sequence.key_sym, key_sequence.mod_mask);
}

bool X11Application::addKeyGrab(const AbstractKeySequence *key_sequence)
{
    return addKeyGrab(*dynamic_cast<const X11Global::KeySequence*>(key_sequence));
}

void X11Application::releaseKeyGrab(const AbstractKeySequence *key_sequence)
{
    releaseKeyGrab(*dynamic_cast<const X11Global::KeySequence*>(key_sequence));
}

bool X11Application::addKeyGrab(const X11Global::KeySequence &key_sequence)
{
    for(std::list<KeyGrab>::iterator it = _key_grabs.begin();
        it != _key_grabs.end();
        it++)
    {
        if (it->key_sequence.equals(key_sequence)) {
            it->ref_count++;
            return true;
        }
    }

    //FIXME: error message / return false, if grabbing is not possible

    XGrabKey(_dpy, XKeysymToKeycode(_dpy, key_sequence.key_sym),
             key_sequence.mod_mask, root(), true, GrabModeAsync, GrabModeAsync);
    XGrabKey(_dpy, XKeysymToKeycode(_dpy, key_sequence.key_sym),
             key_sequence.mod_mask | LockMask, root(), true, GrabModeAsync, GrabModeAsync);
    XGrabKey(_dpy, XKeysymToKeycode(_dpy, key_sequence.key_sym),
             key_sequence.mod_mask | num_lock_mask, root(), true, GrabModeAsync, GrabModeAsync);
    XGrabKey(_dpy, XKeysymToKeycode(_dpy, key_sequence.key_sym),
             key_sequence.mod_mask | LockMask | num_lock_mask, root(), true, GrabModeAsync, GrabModeAsync);

    _key_grabs.push_back(KeyGrab(key_sequence));

    return true;
}

void X11Application::releaseKeyGrab(const X11Global::KeySequence &key_sequence)
{
    for(std::list<KeyGrab>::iterator it = _key_grabs.begin();
        it != _key_grabs.end();
        it++)
    {
        if (it->key_sequence.equals(key_sequence)) {
            it->ref_count--;

            assert(it->ref_count >= 0);

            if (it->ref_count == 0) {
                XUngrabKey(_dpy, XKeysymToKeycode(_dpy, key_sequence.key_sym),
                           key_sequence.mod_mask, root());
                XUngrabKey(_dpy, XKeysymToKeycode(_dpy, key_sequence.key_sym),
                           key_sequence.mod_mask | LockMask, root());
                XUngrabKey(_dpy, XKeysymToKeycode(_dpy, key_sequence.key_sym),
                           key_sequence.mod_mask | num_lock_mask, root());
                XUngrabKey(_dpy, XKeysymToKeycode(_dpy, key_sequence.key_sym),
                           key_sequence.mod_mask | LockMask | num_lock_mask, root());

                _key_grabs.erase(it);

                return;
            }
        }
    }
}

void X11Application::getMonitorSize(int &w, int &h)
{
    w = _root_width;
    h = _root_height;
}

Cursor X11Application::getCursor(unsigned int shape)
{
    Cursor cursor = None;
    auto it = _cursors.find(shape);
    if (it != _cursors.end())
        cursor = it->second;
    else {
        cursor = XcursorShapeLoadCursor(_dpy, shape);
        _cursors.insert(std::pair<unsigned int, Cursor>(shape, cursor));
    }

    return cursor;
}
