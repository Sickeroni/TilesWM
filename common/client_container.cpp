#include "client_container.h"

#include "client.h"
#include "container_container.h"
#include "client_container_layout.h"
#include "canvas.h"
#include "workspace.h"
#include "icon.h"
#include "colors.h"
#include "theme.h"
#include "application.h"
#include "common.h"

#include <sstream>
#include <string>
#include <stdlib.h>
#include <assert.h>


ClientContainer::ClientContainer() : Container(CLIENT),
    _layout(new ClientContainerLayout(this))
{
}

ClientContainer::~ClientContainer()
{
    delete _layout;
    _layout = 0;
}

ContainerLayout *ClientContainer::getLayout()
{
    return _layout;
}

void ClientContainer::handleMouseClick(int global_x, int global_y)
{
    //FIXME move this to mouse handler class

    printvar(global_x);
    printvar(global_y);

    int local_x = global_x;
    int local_y = global_y;
    globalToLocal(local_x, local_y);

    printvar(local_x);
    printvar(local_y);

    int clicked_tab_index = Theme::getTabAt(local_x, local_y, this);

    if (clicked_tab_index != INVALID_INDEX) {
        setActiveChild(clicked_tab_index);
        makeActive();
        Application::self()->setFocus(activeClient());
    }
}

void ClientContainer::handleClientFocusChange(Client *client)
{
    redraw();
}

void ClientContainer::handleClientSizeHintChanged(Client *client)
{
    if (activeClient() == client)
        parent()->handleSizeHintsChanged(this);
    getLayout()->layoutContents();
}

void ClientContainer::redrawAll()
{
    redraw();
}
