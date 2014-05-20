#include "theme.h"

#include "container_container.h"
#include "canvas.h"
#include "colors.h"

#include <sstream>

void Theme::drawContainerContainer(ContainerContainer *container, Canvas *canvas)
{
    Rect bg_rect = container->rect();
    bg_rect.setPos(0, 0);
    canvas->erase(bg_rect);

    Rect title_rect(ContainerContainer::_frame_width - 2, ContainerContainer::_frame_width - 2,
                    container->width() - 4 - (2 * ContainerContainer::_frame_width), ContainerContainer::_title_height);

    std::stringstream title;

    if (container->isHorizontal())
        title<<"H";
    else
        title<<"V";

    if (container->isFixedSize())
        title<<"  - ";
    else
        title<<" <->";

    canvas->drawText(title.str().c_str(),
                     title_rect,
                     container->isActive() ?  Colors::CONTAINER_FOCUS : Colors::CONTAINER_BORDER);

    for (int i = 0; i < container->numElements(); i++) {
        Container *c = container->child(i);
        Rect frame_rect = c->rect();
        frame_rect.x -= 5;
        frame_rect.y -= 5;
        frame_rect.w += 10;
        frame_rect.h += 10;

        uint32 frame_color = c->isClientContainer() ? Colors::CLIENT_CONTAINER_BORDER:
                                                      Colors::CONTAINER_BORDER;

        canvas->drawFrame(frame_rect, frame_color);

        if (c->hasFocus()) {
            Rect focus_rect = frame_rect;
            focus_rect.x += 1;
            focus_rect.y += 1;
            focus_rect.w -= 2;
            focus_rect.h -= 2;

            uint32 color = c->isClientContainer() ? Colors::CLIENT_CONTAINER_FOCUS :
                                                    Colors::CONTAINER_FOCUS;

            canvas->drawFrame(focus_rect, color);
        }
    }
}
