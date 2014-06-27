#ifndef __THEME_H__
#define __THEME_H__

#include "rect.h"

class ContainerBase;
class ContainerContainer;
class ClientContainer;
class Client;
class Canvas;

namespace Theme
{
    struct ContainerContainerSizes
    {
        int child_frame_width;
//         int title_height;
        int frame_width;
    };

    struct ClientContainerSizes
    {
        int min_contents_width;
        int frame_width;
    };

    const ContainerContainerSizes &containerContainerSizes();
    const ClientContainerSizes &clientContainerSizes();

    void drawContainer(ContainerBase *container, Canvas *canvas);

    int calcContainerContainerTitlebarHeight(const ContainerContainer *container);
    inline void getContainerContainerClientRect(const ContainerContainer *container,
                                                const Rect &container_rect,
                                                Rect &client_rect) {
        const ContainerContainerSizes &sizes = containerContainerSizes();
        int title_height = calcContainerContainerTitlebarHeight(container);

        client_rect.set(sizes.frame_width, sizes.frame_width + title_height,
                 container_rect.w - (sizes.frame_width * 2), container_rect.h - title_height - (sizes.frame_width * 2));
    }

    void getClientContainerClientRect(ClientContainer *container, Rect &client_rect);
    int calcTabbarHeight(ClientContainer *container);
    int calcVerticalTabbarHeight(ClientContainer *container);
    int getTabAt(int x, int y, ClientContainer *container);

    void drawClientFrame(Client *client, Canvas *canvas);
    void calcClientFrameRect(Client *client, const Rect &client_rect, Rect &frame_rect);
    void calcClientClientRect(Client *client, const Rect &frame_rect, Rect &client_rect);
}

#endif // __THEME_H__
