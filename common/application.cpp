#include "application.h"

#include "monitor.h"
#include "workspace.h"
#include "client_container.h"
#include "container_container.h"
#include "client.h"
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
    _self = 0;
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

    ContainerContainer *root_container = activeWorkspace()->rootContainer();

    if (!root_container->activeClientContainer()) {
        ClientContainer *c = self()->createClientContainer();
        int index = root_container->addChild(c);
        root_container->setActiveChild(index);
    }

    root_container->activeClientContainer()->addChild(client);
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
