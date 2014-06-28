#include "client_util.h"

#include "client.h"
#include "client_container.h"
#include "workspace.h"
#include "window_manager.h"

namespace ClientUtil
{


void makeActive(Client *client)
{
    //FIXME set active layer
    if (ClientContainer *container = client->container()) {
        container->workspace()->windowManager()->makeContainerActive(container);
        container->setActiveChild(container->indexOfChild(client));
    } else if (client->workspace()) {
        //FIXME
        // workspace->setActiveClient(client);
        client->workspace()->makeActive();
    }
}


} // namespace ClientUtil
