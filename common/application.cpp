#include "application.h"

#include "monitor.h"
#include "workspace.h"
#include "client_container.h"
#include "container_container.h"
#include "shortcut_set.h"
#include "actions.h"
#include "client.h"
#include "mode_default.h"
#include "mode_3panel.h"
#include "common.h"

#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

Application *Application::_self = 0;

Application::Application()
{
    if (_self)
        abort();
    _self = this;
}

Application::~Application()
{
}

void Application::init()
{
    _main_shortcuts = createShortcutSet("main");

    _main_shortcuts->createAction("toggleMaximize", &Actions::toggleMaximize);
    _main_shortcuts->createAction("layout", &Actions::layout);
    _main_shortcuts->createAction("rotate", &Actions::rotate);
    _main_shortcuts->createAction("focusLeft", &Actions::focusLeft);
    _main_shortcuts->createAction("focusRight", Actions::focusRight);
    _main_shortcuts->createAction("focusUp", &Actions::focusUp);
    _main_shortcuts->createAction("focusDown", &Actions::focusDown);
    _main_shortcuts->createAction("redraw", &Actions::redraw);
    _main_shortcuts->createAction("focusPrevClient", &Actions::focusPrevClient);
    _main_shortcuts->createAction("focusNextClient", &Actions::focusNextClient);
    _main_shortcuts->createAction("runProgram", &Actions::runProgram);
    _main_shortcuts->createAction("runTerminal", &Actions::runTerminal);
    _main_shortcuts->createAction("changeMode", &Actions::changeMode);
    _main_shortcuts->createAction("focusActiveClient", &focusActiveClient);
    _main_shortcuts->createAction("closeActiveClient", &Actions::closeActiveClient);
    _main_shortcuts->createAction("quit", &Actions::quit);

    _modes.push_back(new ModeDefault());
    _modes.push_back(new Mode3Panel());

    for(size_t i = 0; i < _modes.size(); i++)
        _modes[i]->initShortcuts();

    if (_default_mode >= _modes.size())
        _default_mode = 0;
}

void Application::shutdown()
{
    _self = 0;

    for(size_t i = 0; i < _modes.size(); i++)
        delete _modes[i];
    _modes.clear();

    delete _main_shortcuts;
    _main_shortcuts = 0;
}

Workspace *Application::activeWorkspace()
{
    return self()->activeMonitor()->workspace();
}

ClientContainer *Application::activeClientContainer()
{
    if (self()->activeLayer() != LAYER_TILED)
        return 0;
    else
        return activeWorkspace()->rootContainer()->activeClientContainer();
}

void Application::manageClient(Client *client, bool is_floating)
{
    assert(!client->container());
    assert(!client->workspace());

    if (is_floating)
        activeWorkspace()->addClient(client);
    else
        activeWorkspace()->mode()->tileClient(client, activeWorkspace()->rootContainer());
}

void Application::unmanageClient(Client *client)
{
    if (client->container())
        client->container()->removeChild(client);
    else if (client->workspace())
        client->workspace()->removeClient(client);
}

void Application::runProgram(const char *path)
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
        cerr<<"ERROR: running "<<path<<": Can't fork.";
}

void Application::focusActiveClient()
{
    //FIXME - floating layer !
    ClientContainer *container = activeClientContainer();
    printvar(container);
    if (container) {
        printvar(container->activeClient());
        self()->setFocus(container->activeClient());
    }
}

Client *Application::activeClient()
{
    if (self()->activeLayer() == LAYER_TILED) {
        if (activeClientContainer())
            return activeClientContainer()->activeClient();
        else
            return 0;
    } else
        return activeWorkspace()->activeClient();

}
