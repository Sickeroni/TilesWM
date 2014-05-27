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
    _main_shortcuts = createShortcutSet();

    _main_shortcuts->createShortcut("MOD1+l", &Actions::layout);
    _main_shortcuts->createShortcut("MOD1+r", &Actions::rotate);
    _main_shortcuts->createShortcut("MOD1+KP_Left", &Actions::focusLeft);
    _main_shortcuts->createShortcut("MOD1+KP_Right", Actions::focusRight);
    _main_shortcuts->createShortcut("MOD1+KP_Up", &Actions::focusUp);
    _main_shortcuts->createShortcut("MOD1+KP_Down", &Actions::focusDown);
    _main_shortcuts->createShortcut("MOD1+t", &Actions::runTerminal);
    _main_shortcuts->createShortcut("MOD1+d", &Actions::redraw);
    _main_shortcuts->createShortcut("MOD1+comma", &Actions::focusPrevClient);
    _main_shortcuts->createShortcut("MOD1+period", &Actions::focusNextClient);
    _main_shortcuts->createShortcut("MOD1+F2", &Actions::runProgram);
    _main_shortcuts->createShortcut("CTRL+m", &Actions::changeMode);
    _main_shortcuts->createShortcut("MOD1+q", &Actions::quit);

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

void Application::tileClient(Client *client)
{
    assert(!client->container());
    assert(!client->workspace());

    activeWorkspace()->mode()->tileClient(client, activeWorkspace()->rootContainer());
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
