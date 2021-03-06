#include "mouse_handler.h"

#include "theme.h"
#include "client_container.h"
#include "window_manager.h"
#include "client_container_theme.h"
#include "workspace.h"
#include "client_wrapper.h"
#include "application.h"

namespace MouseHandler
{


void handleClientContainerClick(ClientContainer *container, int x, int y)
{
    printvar(x);
    printvar(y);

    int clicked_tab_index = Theme::getTabAt(x, y, container);

    if (clicked_tab_index != -1) {
        ClientWrapper *client = container->child(make_unsigned<unsigned int>(clicked_tab_index));
        container->getWorkspace()->makeClientActive(client->wrappedClient());
        client->setFocus();
    }
}


void handleContainerClick(Container *container, int x, int y)
{
    switch(container->type()) {
        case Container::CLIENT:
            handleClientContainerClick(container->toClientContainer(), x, y);
            break;
        default:
            break;
    }
}


} // namespace MouseHandler
