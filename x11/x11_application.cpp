#include "x11_application.h"

#include "x11_container_container.h"
#include "x11_widget.h"

#include <sys/select.h>

#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>


X11Application *X11Application::_self = 0;


X11Application::X11Application() :
    _display(0),
    _root(0),
    _activeRootContainer(0),
    _quit_requested(false)
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

bool X11Application::init()
{
    signal(SIGINT, &quit);

    XWindowAttributes root_attr;
    XSetWindowAttributes new_root_attr;


    /* return failure status if we can't connect */
    if(!(_display = XOpenDisplay(0))) {
        std::cerr << "ERROR: can't open display.\n";
        return false;
    }

    XGrabServer(_display);

    _root = DefaultRootWindow(_display);

    if (!XGetWindowAttributes(_display, _root, &root_attr)) {
        std::cout << "XGetWindowAttributes() failed !\n";
        XCloseDisplay(_display);
        return false;
    }

    memset(&new_root_attr, 0, sizeof(XSetWindowAttributes));

    new_root_attr.event_mask = root_attr.your_event_mask | SubstructureNotifyMask;

    //FIXME reset all attributes ?

    XChangeWindowAttributes(_display, _root, CWEventMask, &new_root_attr);


    Rect root_container_rect;
    root_container_rect.set(0, 0, root_attr.width, root_attr.height);

    _activeRootContainer = new X11ContainerContainer(0);
    _activeRootContainer->setRect(root_container_rect);

    X11Widget::initClientWidgets();

//     XFlush(display());
    XSync(display(), false);

    XUngrabServer(_display);

    std::cout<<"initialisation finished.\n";

    return true;
}

void X11Application::shutdown()
{
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
    KeySym layout_key = XStringToKeysym("l");

    XGrabKey(display(), XKeysymToKeycode(display(), layout_key), Mod1Mask, root(),
            True, GrabModeAsync, GrabModeAsync);

    int x11_fd = ConnectionNumber(display());

    timeval timeout_spec;
    fd_set x11_fd_set;
    XEvent ev;

    for( ; ; )
    {
//         XFlush(display());

        if (_quit_requested)
            return;

//         std::cout << "X11Application::eventLoop(): waiting for event or timeout ...\n";

#if 1
        while (!XPending(display())) {
            // Wait for X Event or a timeout
            FD_ZERO(&x11_fd_set);
            FD_SET(x11_fd, &x11_fd_set);

            timeout_spec.tv_usec = 0;
            timeout_spec.tv_sec = 2;

            int select_ret = select(x11_fd+1, &x11_fd_set, 0, 0, &timeout_spec);

            if (_quit_requested)
                return;
#if 0
            if (select_ret) {
                std::cout << "X11Application::eventLoop(): Event Received!\n";
//                 continue;
            } else if (select_ret == 0) {
                std::cout << "X11Application::eventLoop(): Timeout!\n";
//                 continue;
            } else {
                std::cerr << "X11Application::eventLoop(): ERROR: select() returned " << select_ret << '\n';
//                 continue;
            }
#endif
        }
#endif

        //peek for destroy events first
//         while(XCheckTypedEvent(display(), DestroyNotify, &ev) {
//             X11Widget::destroyNotify(ev.xdestroywindow);
//         }



        //FIXME UGLY
//         if (Container::root()->isContainerContainer())
//             static_cast<ContainerContainer*>(Container::root())->deleteEmptyChildren();



        /* this is the most basic way of looping through X events; you can be
         * more flexible by using XPending(), or ConnectionNumber() along with
         * select() (or poll() or whatever floats your boat).
         */
        XNextEvent(_display, &ev);

//         FIXME do this more efficiently
//         XGrabServer(display());


#if 1
        if(ev.type == CreateNotify)
            X11Widget::createNotify(ev.xcreatewindow);
        else if(ev.type == DestroyNotify)
            X11Widget::destroyNotify(ev.xdestroywindow);
        else if(ev.type == MapNotify) {
            X11Widget::mapNotify(ev.xmap);
        } else if(ev.type == UnmapNotify) {
            X11Widget::unmapNotify(ev.xunmap);
        }
#endif


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
#if 1
        else if (ev.type == KeyPress) {
            if (XLookupKeysym(&ev.xkey, 0) == layout_key) {
//             if (ev.xkey.keycode == layout_key) {
                std::cout << "layout key pressed !\n";
//                 Container::root()->layout();
                activeRootContainer()->layout();
            }
        }
#endif
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
#if 0
        else {
//             printf("event type: %d\n",ev.type);
//             fflush(stdout);
        }
#endif

        XSync(display(), false);

//         XUngrabServer(display());
    }
}
#endif
