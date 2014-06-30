#include "application.h"

#include "monitor.h"
#include "workspace.h"
// #include "main_actions.h"
#include "client.h"
#include "mode.h"
// #include "mode_default.h"
// #include "mode_3panel.h"
// #include "mode_simple.h"
#include "window_manager.h"
#include "config.h"
#include "common.h"

#include <unistd.h>
#include <sys/wait.h>
#include <cstdio>

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
//     _main_actions = new MainActions();

//     _modes.push_back(new ModeDefault());
//     _modes.push_back(new Mode3Panel());
//     _modes.push_back(new ModeSimple());

//     _main_actions->initShortcuts();

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

#if 0
Container *Application::activeContainer()
{
    if (self()->activeLayer() != LAYER_TILED)
        return 0;
    else
        return activeWorkspace()->windowManager()->activeContainer();
}
#endif

void Application::manageClient(WidgetBackend *backend, bool is_floating)
{
//     Client *client = new Client(backend, is_floating);
    Client *client = new Client(backend, true);

    if (client->isFloating())
        activeWorkspace()->addChild(client);
    else
        activeWorkspace()->windowManager()->manageClient(client);
}

void Application::unmanageClient(Widget *frontend)
{
    Client *client = frontend->toClient();

    if (client->parent()) {
        if (client->isFloating()) {
            client->workspace()->removeChild(client);
            assert(!client->parent());
        } else  {
            assert(0);
//             if (Container *container = client->parent()->toContainer())
//                  container->removeChild(container->indexOfChild(client));

        }
    }

    delete client;
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
#if 0
    //FIXME - floating layer !
    ClientContainer *container = activeClientContainer();
    printvar(container);
    if (container) {
        printvar(container->activeClient());
        self()->setFocus(container->activeClient());
    }
#endif
}

Client *Application::activeClient()
{
    if (self()->activeLayer() == LAYER_TILED)
        return activeWorkspace()->windowManager()->activeClient();
    else
        return activeWorkspace()->activeFloatingClient();
}

void Application::makeClientActive(Widget *widget)
{
    ChildWidget *client = widget->toChildWidget();
    assert(client);

    Workspace *workspace = client->workspace();
    assert(workspace);

    if (workspace->makeActive()) {
        if (client->isFloating()) {
            workspace->setActiveFloatingChild(client);
            self()->setActiveLayer(LAYER_FLOATING);
        } else {
            workspace->windowManager()->makeClientActive(client);
            self()->setActiveLayer(LAYER_TILED);
        }
    }
}

void Application::setFocus(Client *client)
{
    client->setFocus();
}
