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
