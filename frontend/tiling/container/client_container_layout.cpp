#include "client_container_layout.h"

#include "client_container.h"
#include "client_container_theme.h"
#include "client_wrapper.h"

#include <cmath>

using std::max;


ClientContainerLayout::ClientContainerLayout(ClientContainer *container) :
    _container(container)
{
}

int ClientContainerLayout::minWidth()
{
    const Theme::ClientContainerSizes &sizes = Theme::clientContainerSizes();

    int min_w = sizes.min_contents_width;
    if (!_container->isMinimized()) {
        if (ClientWrapper *client = _container->activeClient()) {
            if (client->sizeHints().min_width > min_w)
                min_w = client->sizeHints().min_width;
        }
    }
    return min_w + (2 * sizes.frame_width);
}

int ClientContainerLayout::maxWidth()
{
    const Theme::ClientContainerSizes &sizes = Theme::clientContainerSizes();
    int min_width = minWidth();

    if (ClientWrapper *client = _container->activeClient()) {
//         if (!isFixedSize()) {
            if (client->sizeHints().max_width) {
                int max_width = (2 * sizes.frame_width) + client->sizeHints().max_width;
                if (max_width > min_width)
                    return max_width;
                else
                    return min_width;
            } else
                return 0;
//         } else
//             return min_width;
    } else
        return min_width;
}

int ClientContainerLayout::minHeight()
{
    const Theme::ClientContainerSizes &sizes = Theme::clientContainerSizes();
    int tabbar_height = 0;

    tabbar_height = Theme::calcTabbarHeight(_container);

    int min_client_height = 0;
    if (!_container->isMinimized() && _container->activeClient())
        min_client_height = _container->activeClient()->sizeHints().min_height;

    return tabbar_height + min_client_height + (2 * sizes.frame_width);
}

int ClientContainerLayout::maxHeight()
{
    //FIXME

    if (_container->isEmpty())
        return 50; //HACK
    else
        return 0;
}

void ClientContainerLayout::layoutContents()
{
    if (!_container->isMinimized()) {
        Rect client_rect;
        Theme::getClientContainerClientRect(_container, client_rect);

        client_rect.w = max(10, client_rect.w);
        client_rect.h = max(10, client_rect.h);

        //FIXME only resize active client ?
            //problem with that: on X11 the other clients are visible too
            //maybe the backend should defer setting the size until the client becomes visible (possibly in case of wayland)
        for(Container::Index i = 0; i < _container->numElements(); i++)
            _container->child(i)->setRect(client_rect);
    }

    _container->redraw();
}
