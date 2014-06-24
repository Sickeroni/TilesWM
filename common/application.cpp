#include "application.h"

#include "monitor.h"
#include "workspace.h"
#include "client_container.h"
#include "container_container.h"
#include "main_actions.h"
#include "client.h"
#include "mode_default.h"
#include "mode_3panel.h"
#include "window_manager.h"
#include "config.h"
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
    _main_actions = new MainActions();

    _modes.push_back(new ModeDefault());
//     _modes.push_back(new Mode3Panel());

    _main_actions->initShortcuts();

    if (_default_mode >= _modes.size())
        _default_mode = 0;
}

void Application::shutdown()
{
    _self = 0;

    for(size_t i = 0; i < _modes.size(); i++)
        delete _modes[i];
    _modes.clear();

    delete _main_actions;
    _main_actions = 0;
}

void Application::reloadConfig()
{
    //TODO ConfigChangedListener

    Config::self()->reload();

    _main_actions->reloadShortcuts();

    //FIXME: foreach(Workspace *w) w->windowManager()->reloadShortcuts()
}

const ShortcutSet *Application::mainShortcuts()
{
    return _main_actions->shortcuts();
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
        return activeWorkspace()->windowManager()->activeClientContainer();
}

void Application::manageClient(Client *client, bool is_floating)
{
    assert(!client->container());
    assert(!client->workspace());

    if (is_floating)
        activeWorkspace()->addClient(client);
    else
        activeWorkspace()->windowManager()->manageClient(client);
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
