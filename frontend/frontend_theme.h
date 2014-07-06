#ifndef __FRONTEND_THEME_H__
#define __FRONTEND_THEME_H__

#include "rect.h"

class ContainerBase;
class ContainerContainer;
class ClientContainer;
class Client;
class Canvas;
class Widget;

namespace Theme
{
#if 0
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
#endif

    void drawWidget(Widget *widget, Canvas *canvas);

#if 0
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
#endif
}

#endif // __THEME_H__
