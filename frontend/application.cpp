#include "application.h"

#include "monitor.h"
#include "workspace.h"
#include "common_actions.h"
// #include "client.h"
#include "mode.h"
// #include "mode_default.h"
// #include "mode_3panel.h"
// #include "mode_simple.h"
#include "window_manager.h"
#include "config.h"
#include "common.h"

// #include "../frontend/mode_simple.h"
// #include "../frontend/main_actions.h"

#include <unistd.h>
#include <sys/wait.h>
#include <cstdio>
#include <cstring>

Application *Application::_self = 0;

Application::Application()
{
    if (_self)
        abort();
    _self = this;
}

Application::~Application()
{
    _self = 0;
}

void Application::init(Backend *backend)
{
    _backend = backend;

    _common_actions = new CommonActions();

//     _modes.push_back(new ModeDefault());
//     _modes.push_back(new Mode3Panel());
//     _modes.push_back(new ModeSimple());

    _common_actions->createKeyBindings();

    if (_default_mode >= _modes.size())
        _default_mode = 0;

//     _monitor = new Monitor();
//     _monitor->setSize(root_attr.width, root_attr.height);
}

void Application::shutdown()
{
#if 0
    delete _monitor;
    _monitor = 0;

    for (size_t i = 0; i < _workspaces.size(); i++) {
        Workspace *w = _workspaces[i];
        _workspaces[i] = 0;
        delete w;
    }
    _workspaces.clear();
#endif

    for(size_t i = 0; i < _modes.size(); i++)
        delete _modes[i];
    _modes.clear();

    delete _common_actions;
    _common_actions = 0;

    _backend = 0;
}

void Application::focusActiveClient()
{
    UNIMPLEMENTED
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

ClientFrontend *Application::createClientFrontend(ClientBackend *backend, bool is_floating)
{
    UNIMPLEMENTED
    return 0;
}

void Application::destroyClientFrontend(ClientFrontend *frontend) 
{
    UNIMPLEMENTED
}

int Application::numKeyGrabHandlers()
{
    return 1;
}

KeyGrabHandlerBase *Application::keyGrabHandler(int index)
{
    assert(index < numKeyGrabHandlers());
    switch (index) {
        case 0:
            return  _common_actions;
            break;
        default:
            assert(false);
            return 0;
    }
}

void Application::reloadConfig()
{
    //TODO ConfigChangedListener

    Config::self()->reload();

//     _main_actions->reloadShortcuts();

    //FIXME: foreach(Workspace *w) w->windowManager()->reloadShortcuts()
}

// const ShortcutSet *Application::mainShortcuts()
// {
//     return _main_actions->shortcuts();
// }

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

// void Application::manageClient(WidgetBackend *backend, bool is_floating)
// {
// //     Client *client = new Client(backend, is_floating);
//     Client *client = new Client(backend, true);
// 
//     if (client->isFloating())
//         activeWorkspace()->addChild(client);
//     else
//         activeWorkspace()->windowManager()->manageClient(client);
// }
// 
// void Application::unmanageClient(Widget *frontend)
// {
//     Client *client = frontend->toClient();
// 
//     if (client->parent()) {
//         if (client->isFloating()) {
//             client->workspace()->removeChild(client);
//             assert(!client->parent());
//         } else  {
//             assert(0);
// //             if (Container *container = client->parent()->toContainer())
// //                  container->removeChild(container->indexOfChild(client));
// 
//         }
//     }
// 
//     delete client;
// }

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

void Application::runProgram(const std::vector<std::string> &args)
{
    assert(!args.empty());
    assert(!args[0].empty());

    const char *program = args[0].c_str();

    char *argv[args.size() + 1];
    memset(argv, 0, sizeof(argv));

    for (size_t i = 0; i < args.size(); i++)
        argv[i] = const_cast<char*>(args[i].c_str());

    pid_t pid = fork();

    if (pid > 0) {
        waitpid(pid, 0, WNOHANG);
    } else if (pid == 0) {
        execvp(program, argv);
        printvar(program);
        perror ("ERROR: Can't execute program: ");
        exit(1);
    } else
        cerr<<"ERROR: running "<<program<<": Can't fork.";
}

// Widget *Application::activeClient()
// {
//     if (self()->activeLayer() == LAYER_TILED)
//         return activeWorkspace()->windowManager()->activeClient();
//     else
//         return activeWorkspace()->activeFloatingClient();
// }

// void Application::makeClientActive(Widget *widget)
// {
//     ChildWidget *client = widget->toChildWidget();
//     assert(client);
// 
//     Workspace *workspace = client->workspace();
//     assert(workspace);
// 
//     if (workspace->makeActive()) {
//         if (client->isFloating()) {
//             workspace->setActiveFloatingChild(client);
//             self()->setActiveLayer(LAYER_FLOATING);
//         } else {
//             workspace->windowManager()->makeClientActive(client);
//             self()->setActiveLayer(LAYER_TILED);
//         }
//     }
// }

// void Application::setFocus(Client *client)
// {
//     client->setFocus();
// }

