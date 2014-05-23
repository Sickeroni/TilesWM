#include "x11_application.h"

#include "common.h"

#include <X11/Xlib.h>
#include <X11/keysym.h>

#include <string.h>


int main()
{
    X11Application app;

    if (!app.init())
        return 1;

    app.eventLoop();

    cout << "returned from main loop - shutting down ...\n";

    app.shutdown();

    return 0;
}
