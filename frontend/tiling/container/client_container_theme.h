#ifndef __CLIENT_CONTAINER_THEME_H__
#define __CLIENT_CONTAINER_THEME_H__

#include "rect.h"

class ClientContainer;
class Canvas;

namespace Theme
{
    struct ClientContainerSizes
    {
        int min_contents_width;
        int frame_width;
    };

    const ClientContainerSizes &clientContainerSizes();

    void getClientContainerClientRect(ClientContainer *container, Rect &client_rect);
    int calcTabbarHeight(ClientContainer *container);
    int getTabAt(int x, int y, ClientContainer *container);

    void drawClientContainer(ClientContainer *container, Canvas *canvas);
}

#endif
