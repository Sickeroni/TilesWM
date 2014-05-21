#ifndef __THEME_H__
#define __THEME_H__

#include "rect.h"

class ContainerContainer;
class ClientContainer;
class Canvas;

namespace Theme
{
    struct ContainerContainerSizes
    {
        int child_frame_width;
        int title_height;
        int frame_width;
    };

    struct ClientContainerSizes
    {
        int min_contents_width;
        int frame_width;
    };

    const ContainerContainerSizes &containerContainerSizes();
    const ClientContainerSizes &clientContainerSizes();

    void drawContainerContainer(ContainerContainer *container, Canvas *canvas);
    void drawClientContainer(ClientContainer *container, Canvas *canvas);

    inline void getContainerContainerClientRect(const Rect &container_rect, Rect &client_rect) {
        const ContainerContainerSizes &sizes = containerContainerSizes();

        client_rect.set(sizes.frame_width, sizes.frame_width + sizes.title_height,
                 container_rect.w - (sizes.frame_width * 2), container_rect.h - sizes.title_height - (sizes.frame_width * 2));
    }

    void getClientContainerClientRect(ClientContainer *container, Rect &client_rect);
    int calcTabbarHeight(ClientContainer *container);
    int calcVerticalTabbarHeight(ClientContainer *container);
    int getTabAt(int x, int y, ClientContainer *container);
}

#endif // __THEME_H__
