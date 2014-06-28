#include "mouse_handler.h"

#include "theme.h"
#include "client_container.h"
#include "window_manager.h"
#include "workspace.h"
#include "application.h"

namespace MouseHandler
{


void handleClientContainerClick(ClientContainer *container, int x, int y)
{
    printvar(x);
    printvar(y);

    int clicked_tab_index = Theme::getTabAt(x, y, container);

    if (clicked_tab_index != INVALID_INDEX) {
        container->setActiveChild(clicked_tab_index);
        container->workspace()->windowManager()->makeContainerActive(container);
        Application::self()->setFocus(container->activeClient());
    }
}


void handleContainerClick(ContainerBase *container, int x, int y)
{
    switch(container->type()) {
        case ContainerBase::CLIENT:
            handleClientContainerClick(container->toClientContainer(), x, y);
            break;
        default:
            break;
    }
}


} // namespace MouseHandler
