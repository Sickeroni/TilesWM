/* TinyWM is written by Nick Welch <mack@incise.org>, 2005.
 *
 * This software is in the public domain
 * and is provided AS IS, with NO WARRANTY. */

/* much of tinywm's purpose is to serve as a very basic example of how to do X
 * stuff and/or understand window managers, so i wanted to put comments in the
 * code explaining things, but i really hate wading through code that is
 * over-commented -- and for that matter, tinywm is supposed to be as concise
 * as possible, so having lots of comments just wasn't really fitting for it.
 * i want tinywm.c to be something you can just look at and go "wow, that's
 * it?  cool!"  so what i did was just copy it over to annotated.c and comment
 * the hell out of it.  ahh, but now i have to make every code change twice!
 * oh well.  i could always use some sort of script to process the comments out
 * of this and write it to tinywm.c ... nah.
 */

/* most X stuff will be included with Xlib.h, but a few things require other
 * headers, like Xmd.h, keysym.h, etc.
 */


#if 0

int main()
{
    return 0;
}


#else

#include "x11_application.h"

// #include "../container_container.h"
// #include "x11_client.h"

#include <X11/Xlib.h>
#include <X11/keysym.h>

#include <iostream>
// #include <stdio.h>
#include <string.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))

#if 0
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


// KeySym layout_key = NoSymbol;



int main()
{
    X11Application app;

    if (!app.init())
        return 1;

//     Container::startup(root_attr.width, root_attr.height);
//     X11Client::startup();

#if 0

    /* you could also include keysym.h and use the XK_F1 constant instead of
     * the call to XStringToKeysym, but this method is more "dynamic."  imagine
     * you have config files which specify key bindings.  instead of parsing
     * the key names and having a huge table or whatever to map strings to XK_*
     * constants, you can just take the user-specified string and hand it off
     * to XStringToKeysym.  XStringToKeysym will give you back the appropriate
     * keysym or tell you if it's an invalid key name.
     *
     * a keysym is basically a platform-independent numeric representation of a
     * key, like "F1", "a", "b", "L", "5", "Shift", etc.  a keycode is a
     * numeric representation of a key on the keyboard sent by the keyboard
     * driver (or something along those lines -- i'm no hardware/driver expert)
     * to X.  so we never want to hard-code keycodes, because they can and will
     * differ between systems.
     */
    XGrabKey(dpy, XKeysymToKeycode(dpy, XStringToKeysym("F1")), Mod1Mask, root,
            True, GrabModeAsync, GrabModeAsync);


    XGrabKey(dpy, XKeysymToKeycode(dpy, layout_key), Mod1Mask, root,
            True, GrabModeAsync, GrabModeAsync);

    /* XGrabKey and XGrabButton are basically ways of saying "when this
     * combination of modifiers and key/button is pressed, send me the events."
     * so we can safely assume that we'll receive Alt+F1 events, Alt+Button1
     * events, and Alt+Button3 events, but no others.  You can either do
     * individual grabs like these for key/mouse combinations, or you can use
     * XSelectInput with KeyPressMask/ButtonPressMask/etc to catch all events
     * of those types and filter them as you receive them.
     */
    XGrabButton(dpy, 1, Mod1Mask, root, True, ButtonPressMask, GrabModeAsync,
            GrabModeAsync, None, None);
    XGrabButton(dpy, 3, Mod1Mask, root, True, ButtonPressMask, GrabModeAsync,
            GrabModeAsync, None, None);




    GC gc = XCreateGC(dpy, root, 0, 0);
    if(!gc)
        abort();

    XSetForeground(dpy, gc, 0xFFFFFF);

//     char hello_world[] = "Hello World !";

    //     XTextItem text;
//     text.chars = hello_world;
//     text.nchars = strlen(hello_world);
//     text.delta = 0;
//     text.font = None;

//     XDrawImageString(dpy, root, gc, 100, 100, hello_world, strlen(hello_world));

//     XDrawRectangle(dpy, root, gc, 100, 100, 300, 600);


    Window test_window = XCreateSimpleWindow(dpy, root,
                                             10, 10, 200, 300, 2, 0xFFFFFF, 0);
#endif
    app.eventLoop();

    std::cout << "returnd from main loop - shutting down ...\n";
//     X11Client::shutdown();
//     Container::shutdown();

    app.shutdown();



    return 0;
}
#endif

