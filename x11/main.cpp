#include "x11_application.h"

#include "common.h"

int main()
{
    X11Application app;

    if (!app.init())
        return 1;

    app.eventLoop();

    cout << "returned from main loop - shutting down ...\n";

    app.shutdown();

    cout << "shutdown finished. goodbye.\n";

    return 0;
}
