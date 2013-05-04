#include "x11_application.h"

#include "x11_container_container.h"
#include "x11_client.h"
#include "x11_widget.h"
#include "x11_default_key_handler.h"

#include "workspace.h"
#include "client_container.h"

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


X11Application::X11Application() :
    _display(0),
    _root(0),
    _key_handler(0),
    _num_server_grabs(0),
    _quit_requested(false),
    _workspace(0)
{
    if (_self)
        abort();
    _self = this;
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
    signal(SIGINT, &quit);

//     XSetErrorHandler(&xErrorHandler);

    /* return failure status if we can't connect */
    if(!(_display = XOpenDisplay(0))) {
        std::cerr << "ERROR: Can't open display.\n";
        return false;
    }

    XSynchronize(_display, true);

    grabServer();

    _root = DefaultRootWindow(_display);

    XWindowAttributes root_attr;
    if (!XGetWindowAttributes(_display, _root, &root_attr)) {
        std::cerr << "ERROR: XGetWindowAttributes() failed for root window.\n";
        XCloseDisplay(_display);
        _display = 0;
        return false;
    }

    if (root_attr.all_event_masks & SubstructureRedirectMask) {
        std::cerr<<"ERROR: Another window manager is already running.\n";
        XCloseDisplay(_display);
        _display = 0;
        return false;
    }

    XSetWindowAttributes new_root_attr;
    memset(&new_root_attr, 0, sizeof(XSetWindowAttributes));

    new_root_attr.event_mask = SubstructureNotifyMask | SubstructureRedirectMask;

    XChangeWindowAttributes(_display, _root, CWEventMask, &new_root_attr);

    XSync(_display, false);

    _workspace = new Workspace();

    Rect root_container_rect;
    root_container_rect.set(0, 0, root_attr.width, root_attr.height);

    _workspace->_root_container = X11ContainerContainer::create(_workspace);

    activeRootContainer()->setRect(root_container_rect);

    activeRootContainer()->addNewClientContainer(false); //FIXME HACK
//    _activeRootContainer->addNewClientContainer(false); //FIXME HACK

    X11Client::init();

    _key_handler = new X11DefaultKeyHandler();

//     XFlush(display());
    XSync(_display, false);

    ungrabServer();

    std::cout<<"initialisation finished.\n";

    return true;
}

void X11Application::shutdown()
{
    //FIXME delete root container
    delete _key_handler;
    _key_handler = 0;
    XCloseDisplay(_display);
    _display = 0;
}

#if 1
void X11Application::eventLoop()
{
    std::cout<<"------------------------------\n";
    std::cout<<"event loop started.\n";
    std::cout<<"------------------------------\n";

    
//     XWindowAttributes attr;

    /* we use this to save the pointer's state at the beginning of the
     * move/resize.
     */
//     XButtonEvent start;


    int x11_fd = ConnectionNumber(display());

    timeval timeout_spec;
    fd_set x11_fd_set;
    XEvent ev;

    for( ; ; )
    {
//         XFlush(display());

        if (_quit_requested)
            return;

        while (!XPending(display())) {
            // Wait for X Event or a timeout
            FD_ZERO(&x11_fd_set);
            FD_SET(x11_fd, &x11_fd_set);

            timeout_spec.tv_usec = 0;
            timeout_spec.tv_sec = 2;

            select(x11_fd+1, &x11_fd_set, 0, 0, &timeout_spec);

            if (_quit_requested)
                return;
        }


        //FIXME UGLY
//         if (Container::root()->isContainerContainer())
//             static_cast<ContainerContainer*>(Container::root())->deleteEmptyChildren();



        /* this is the most basic way of looping through X events; you can be
         * more flexible by using XPending(), or ConnectionNumber() along with
         * select() (or poll() or whatever floats your boat).
         */
        XNextEvent(_display, &ev);

        if (ev.type == KeyPress && X11Shortcut::handleKeyPress(ev.xkey)) {
            // NO-OP
        }

        else if (ev.xany.window && X11Widget::handleEvent(ev)) {
            // NO-OP
        }


        /* this is our keybinding for raising windows.  as i saw someone
         * mention on the ratpoison wiki, it is pretty stupid; however, i
         * wanted to fit some sort of keyboard binding in here somewhere, and
         * this was the best fit for it.
         *
         * i was a little confused about .window vs. .subwindow for a while,
         * but a little RTFMing took care of that.  our passive grabs above
         * grabbed on the root window, so since we're only interested in events
         * for its child windows, we look at .subwindow.  when subwindow ==
         * None, that means that the window the event happened in was the same
         * window that was grabbed on -- in this case, the root window.
         */

//         else if(ev.type == KeyPress && ev.xkey.subwindow != None)
//             XRaiseWindow(_display, ev.xkey.subwindow);
#if 0
        else if(ev.type == ButtonPress && ev.xbutton.subwindow != None)
        {
            /* now we take command of the pointer, looking for motion and
             * button release events.
             */
            XGrabPointer(_display, ev.xbutton.subwindow, True,
                    PointerMotionMask|ButtonReleaseMask, GrabModeAsync,
                    GrabModeAsync, None, None, CurrentTime);

            /* we "remember" the position of the pointer at the beginning of
             * our move/resize, and the size/position of the window.  that way,
             * when the pointer moves, we can compare it to our initial data
             * and move/resize accordingly.
             */
            XGetWindowAttributes(_display, ev.xbutton.subwindow, &attr);
            start = ev.xbutton;
        }
#endif
        /* the only way we'd receive a motion notify event is if we already did
         * a pointer grab and we're in move/resize mode, so we assume that. */
#if 0
        else if(ev.type == MotionNotify)
        {
            int xdiff, ydiff;

            /* here we "compress" motion notify events.  if there are 10 of
             * them waiting, it makes no sense to look at any of them but the
             * most recent.  in some cases -- if the window is really big or
             * things are just acting slowly in general -- failing to do this
             * can result in a lot of "drag lag."
             *
             * for window managers with things like desktop switching, it can
             * also be useful to compress EnterNotify events, so that you don't
             * get "focus flicker" as windows shuffle around underneath the
             * pointer.
             */
            while(XCheckTypedEvent(_display, MotionNotify, &ev));

            /* now we use the stuff we saved at the beginning of the
             * move/resize and compare it to the pointer's current position to
             * determine what the window's new size or position should be.
             *
             * if the initial button press was button 1, then we're moving.
             * otherwise it was 3 and we're resizing.
             *
             * we also make sure not to go negative with the window's
             * dimensions, resulting in "wrapping" which will make our window
             * something ridiculous like 65000 pixels wide (often accompanied
             * by lots of swapping and slowdown).
             *
             * even worse is if we get "lucky" and hit a width or height of
             * exactly zero, triggering an X error.  so we specify a minimum
             * width/height of 1 pixel.
             */
            xdiff = ev.xbutton.x_root - start.x_root;
            ydiff = ev.xbutton.y_root - start.y_root;
            XMoveResizeWindow(_display, ev.xmotion.window,
                attr.x + (start.button==1 ? xdiff : 0),
                attr.y + (start.button==1 ? ydiff : 0),
                MAX(1, attr.width + (start.button==3 ? xdiff : 0)),
                MAX(1, attr.height + (start.button==3 ? ydiff : 0)));
        }
#endif
        /* like motion notifies, the only way we'll receive a button release is
         * during a move/resize, due to our pointer grab.  this ends the
         * move/resize.
         */
#if 0
        else if(ev.type == ButtonRelease)
            XUngrabPointer(_display, CurrentTime);
#endif
#if 1
        else {
//             std::cout<<"unhandled event: "<<eventTypeToString(ev.type)<<'\n';
        }
#endif

        XSync(display(), false);

//         XUngrabServer(display());
    }
}
#endif

void X11Application::grabServer()
{
    assert(_num_server_grabs >= 0);

    if (!_num_server_grabs)
        XGrabServer(_display);

    _num_server_grabs++;
}

void X11Application::ungrabServer()
{
    _num_server_grabs--;

    assert(_num_server_grabs >= 0);

    if (!_num_server_grabs)
        XUngrabServer(_display);
}

void X11Application::runProgram(const char *path)
{
    char *const args[] = {
        const_cast<char*>(path),
        0
    };

    pid_t pid = fork();

    if (pid > 0) {
        waitpid(pid, 0, WNOHANG);
    } else if (pid == 0) {
        execv(path, args);
        perror ("ERROR: Can't execute program: ");
        exit(1);
    } else
        std::cerr<<"ERROR: running "<<path<<": Can't fork.";
}

X11ContainerContainer *X11Application::activeRootContainer()
{
    return static_cast<X11ContainerContainer*>(activeWorkspace()->rootContainer());
}
