#include "container_container_theme.h"
#include "container_container.h"
#include "workspace.h"
#include "canvas.h"
#include "colors.h"

#include <sstream>


namespace Theme
{


const ContainerContainerSizes _containerContainerSizes = {
    .child_frame_width = 10,
    .frame_width = 10
};

const ContainerContainerSizes &containerContainerSizes()
{
    return _containerContainerSizes;
}

int calcContainerContainerTitlebarHeight(const ContainerContainer *container)
{
    return container->maxTextHeight() + 4; //FIXME
}

void drawContainerContainer(ContainerContainer *container, Canvas *canvas)
{
    const ContainerContainerSizes &sizes = _containerContainerSizes;
    const int title_height = calcContainerContainerTitlebarHeight(container);

    Rect bg_rect = container->rect();
    bg_rect.setPos(0, 0);
    canvas->erase(bg_rect);

    Rect title_rect(sizes.frame_width + 2,
                    sizes.frame_width + 2,
                    container->width() - 4 - (2 * sizes.frame_width),
                    title_height);

    std::stringstream title;

    if (container->isHorizontal())
        title<<"H";
    else
        title<<"V";

    if (container->isFixedSize())
        title<<"  - ";
    else
        title<<" <->";

//     if (container->isMaximized())
//         title<<" +";
//     else
    if (container->isMinimized())
        title<<" -";

    bool is_active = container->isActive();

    canvas->drawText(title.str().c_str(),
                     title_rect,
                     is_active ?  Colors::CONTAINER_FOCUS : Colors::CONTAINER_BORDER);

    for (int i = 0; i < container->numElements(); i++) {
        Container *c = container->child(i);
        Rect frame_rect = c->rect();
        frame_rect.x -= 5;
        frame_rect.y -= 5;
        frame_rect.w += 10;
        frame_rect.h += 10;

        uint32_t frame_color = c->isClientContainer() ? Colors::CLIENT_CONTAINER_BORDER:
                                                      Colors::CONTAINER_BORDER;

        canvas->drawFrame(frame_rect, frame_color);

        if (c->isActive()) {
            Rect focus_rect = frame_rect;
            focus_rect.x += 1;
            focus_rect.y += 1;
            focus_rect.w -= 2;
            focus_rect.h -= 2;

            uint32_t color = c->isClientContainer() ? Colors::CLIENT_CONTAINER_FOCUS :
                                                    Colors::CONTAINER_FOCUS;

            canvas->drawFrame(focus_rect, color);
        }
    }
}


} // namespace Theme
