#ifndef __CONTAINER_UTIL_H__
#define __CONTAINER_UTIL_H__

#include "container.h"

namespace ContainerUtil
{
    enum Direction {
        LEFT,
        RIGHT,
        UP,
        DOWN
    };

    inline bool isForwardDirection(Direction dir) {
        return (dir == RIGHT || dir == DOWN);
    }

    inline Container::Orientation orientationOfDirection(Direction dir)
    {
        if (dir == LEFT || dir == RIGHT)
            return Container::HORIZONTAL;
        else
            return Container::VERTICAL;
    }

    ClientContainer *getSibling(Container *container, bool get_prev, bool create_new_if_not_existing);
    // return: the newly created sibling
    ClientContainer *splitContainer(ClientContainer *container, bool prepend);
}

#endif
