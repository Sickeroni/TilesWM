#include "application.h"

#include "monitor.h"
#include "workspace.h"
#include "client_container.h"
#include "container_container.h"
#include "client.h"
#include "common.h"

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
