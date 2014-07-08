#ifndef __CONTAINER_CONTAINER_THEME_H__
#define __CONTAINER_CONTAINER_THEME_H__

#include "rect.h"

class ContainerContainer;
class Canvas;

namespace Theme
{
    struct ContainerContainerSizes
    {
        int child_frame_width;
        int frame_width;
    };

    const ContainerContainerSizes &containerContainerSizes();

    int calcContainerContainerTitlebarHeight(const ContainerContainer *container);
    inline void getContainerContainerClientRect(const ContainerContainer *container,
                                                const Rect &container_rect,
                                                Rect &client_rect) {
        const ContainerContainerSizes &sizes = containerContainerSizes();
        int title_height = calcContainerContainerTitlebarHeight(container);

        client_rect.set(sizes.frame_width, sizes.frame_width + title_height,
                 container_rect.w - (sizes.frame_width * 2), container_rect.h - title_height - (sizes.frame_width * 2));
    }

    void drawContainerContainer(ContainerContainer *container, Canvas *canvas);
}

#endif
