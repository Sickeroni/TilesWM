#include "client_container_layout.h"

#include "client_container.h"
#include "client.h"
#include "theme.h"

ClientContainerLayout::ClientContainerLayout(ClientContainer *container) :
    _container(container)
{
}

int ClientContainerLayout::minWidth()
{
    const Theme::ClientContainerSizes &sizes = Theme::clientContainerSizes();

    int min_w = sizes.min_contents_width;
    if (!_container->isMinimized() && _container->activeClient() && _container->activeClient()->minWidth() > min_w)
        min_w = _container->activeClient()->minWidth();
    return min_w + (2 * sizes.frame_width);
}

int ClientContainerLayout::maxWidth()
{
    const Theme::ClientContainerSizes &sizes = Theme::clientContainerSizes();
    int min_width = minWidth();

    if (_container->activeClient() && !_container->isMinimized()) {
//         if (!isFixedSize()) {
            if (_container->activeClient()->maxWidth()) {
                int max_width = (2 * sizes.frame_width) + _container->activeClient()->maxWidth();
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

    if (_container->isMinimized()) {
        if (_container->isHorizontal())
            tabbar_height = Theme::calcTabbarHeight(_container);
        else // vertical tabbar
            tabbar_height = Theme::calcVerticalTabbarHeight(_container);
    } else
        tabbar_height = Theme::calcTabbarHeight(_container);
    
    int min_client_height = 0;
    if (!_container->isMinimized() && _container->activeClient())
        min_client_height = _container->activeClient()->minHeight();

    return tabbar_height + min_client_height + (2 * sizes.frame_width);
}

int ClientContainerLayout::maxHeight()
{
    //FIXME
    return 0;
}

void ClientContainerLayout::layoutContents()
{
    if (!_container->isMinimized()) {
        Rect client_rect;
        Theme::getClientContainerClientRect(_container, client_rect);

        //FIXME only resize active client ?
            //problem with that: on X11 the other clients are visible too
            //maybe the backend should defer setting the size until the client becomes visible (possibly in case of wayland)
        for(int i = 0; i < _container->numElements(); i++)
            _container->child(i)->setRect(client_rect);
    }

    _container->redraw();
}
