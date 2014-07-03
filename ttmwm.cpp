#include "backend_x11/x11_application.h"
#include "frontend/application.h"

#include "common.h"

int main()
{
    Application frontend;

    X11Application backend(&frontend);

    if (!backend.init())
        return 1;

    backend.eventLoop();

    cout << "returned from main loop - shutting down ...\n";

    backend.shutdown();

    cout << "shutdown finished. goodbye.\n";

    return 0;
}
