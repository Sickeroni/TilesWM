#ifndef __CONTAINER_UTIL_H__
#define __CONTAINER_UTIL_H__

#include "container.h"

#include <vector>

class ContainerContainer;

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
    // removes all children and puts the de-parented clients into <clients>
//     void emptyContainer(ContainerContainer *container, std::vector<Client*> &clients);

    void deleteEmptyChildren(ContainerContainer *container);
}

#endif
