#include "application.h"

#include "monitor.h"
#include "workspace.h"
#include "common_actions.h"
#include "client.h"
#include "mode.h"
#include "window_manager.h"
#include "backend.h"
#include "config.h"
#include "common.h"

#include <unistd.h>
#include <sys/wait.h>
#include <cstdio>
#include <cstring>

Application *Application::_self = 0;

Application::Application(const std::vector<Mode*> *modes) :
    _modes(modes)
{
}

Application::~Application()
{
}

void Application::init(Backend *backend)
{
    assert(!_self);
    _self = this;

    _backend = backend;

    _common_actions = new CommonActions();

    _common_actions->createKeyBindings();

    for (Mode *mode : *_modes)
        mode->createKeyBindings();

    if (_default_mode >= _modes->size())
        _default_mode = 0;

    int monitor_w, monitor_h;
    backend->getMonitorSize(monitor_w,  monitor_h);

    _monitor = new Monitor();
    _monitor->setSize(monitor_w, monitor_h);
}

void Application::shutdown()
{
    delete _monitor;
    _monitor = 0;

    for (size_t i = 0; i < _workspaces.size(); i++) {
        Workspace *w = _workspaces[i];
        _workspaces[i] = 0;
        delete w;
    }
    _workspaces.clear();

    for (Mode *mode : *_modes)
        mode->clearKeyBindings();

    delete _common_actions;
    _common_actions = 0;

    _backend = 0;

    _self = 0;
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
    printvar(is_floating);
    return new Client(backend, is_floating);
}

void Application::destroyClientFrontend(ClientFrontend *frontend) 
{
    Client *client = dynamic_cast<Client*>(frontend);
    unmanageClient(client);
    delete client;
}

int Application::numKeyGrabHandlers()
{
    return 2;
}

KeyGrabHandlerBase *Application::keyGrabHandler(int index)
{
    assert(index < numKeyGrabHandlers());
    switch (index) {
        case 0:
            return  _common_actions;
            break;
        case 1:
            return activeWorkspace()->windowManager();
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

Workspace *Application::activeWorkspace()
{
    return self()->activeMonitor()->workspace();
}

void Application::manageClient(Client *client)
{
    debug;
    printvar(client->isFloating());
    if (client->isFloating())
        activeWorkspace()->addChild(client);
    else
        activeWorkspace()->windowManager()->manageClient(client);
}

void Application::unmanageClient(Client *client)
{
    if (client->parent()) {
        // FIXME
        // crashes if Workspace::windowManager() is a floating wm
        if (client->isFloating()) {
            client->workspace()->removeChild(client);
            assert(!client->parent());
        } else  {
            client->workspace()->windowManager()->unmanageClient(client);
        }
    }
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

Client *Application::activeClient()
{
    if (self()->activeLayer() == LAYER_TILED)
        return activeWorkspace()->windowManager()->activeClient();
    else
        return activeWorkspace()->activeFloatingClient();
}

void Application::makeClientActive(Client *client)
{
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

void Application::setActiveMonitor(Monitor *monitor)
{
    UNIMPLEMENTED
}

Workspace *Application::createWorkspace()
{
    Workspace *w = new Workspace();
    _workspaces.push_back(w);
    return w;
}
