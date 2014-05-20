#include "layout.h"

#include "container_container.h"
#include "workspace.h"
#include "common.h"

void Layout::layoutContainer(ContainerContainer *container)
{
    static const bool respect_size_hints = true;

    struct LayoutItem
    {
        void init(Container *c, int min, int max) {
            container = c;
            min_size = min;
            max_size = max;

            if (max_size && max_size < min_size) // normalize
                max_size = min_size;

            min_size += (2 * ContainerContainer::_child_frame_width); //FIXME
            if (max_size)
                max_size += (2 * ContainerContainer::_child_frame_width);

            size = min_size;

//             extra_space = 0;
        }

        bool canGrow() {
            return !max_size || size < max_size;
        }

        Container *container;
        int size;
        int min_size, max_size;
//         int extra_space;
    };


    const int num_children = container->numChildren();
    const bool is_horizontal = container->isHorizontal();

    if (!container->width() || !container->height() || !num_children)
        return;

    Rect client_rect;
    container->getClientRect(client_rect);

    if (!client_rect.w || !client_rect.h)
        return;

    printvar(num_children);

    int available_space = is_horizontal ? client_rect.w : client_rect.h;
    printvar(available_space);

    int num_growable_items = num_children;

    // create layout item for each child
    LayoutItem *layout_items = new LayoutItem[num_children];
    // initialize layout items
    {
        for (int i = 0 ; i < num_children; i++) {
            Container *c = container->child(i);

            int min_size, max_size;
            if (!respect_size_hints) // FIXME move this flag to Container and test against in it Container::minSize()/maxSize()
                min_size = max_size = 0;
            else if (is_horizontal) {
                min_size = c->minWidth();
                max_size = c->maxWidth();
            } else {
                min_size = c->minHeight();
                max_size = c->maxHeight();
            }

            if (c->isFixedSize()) {
                //FIXME - make sure min_size <= c->[maxWidth()|maxHeight()]
                if (is_horizontal) {
                    min_size = c->minWidth() + c->fixedWidth();
                } else {
                    min_size = c->minHeight() + c->fixedHeight();
                }
                max_size = min_size;
            }


            LayoutItem &item = layout_items[i];
            item.init(c, min_size, max_size);

            debug<<"item"<<i<<"size:"<<item.size;
            debug<<"item"<<i<<"min size:"<<item.min_size;
            debug<<"item"<<i<<"max size:"<<item.max_size;

            available_space -= item.size;

            if(!item.canGrow())
                num_growable_items--;
        }
    }

    debug<<"done distributing minimum sizes.";
    printvar(available_space);

    if (available_space < 0) // BAAD - children won't fit
        available_space = 0;

    if (!(container->workspace()->maximized())) {
        // distribute remaining available space
        debug<<"distributing remaining available space ...";
        while (available_space && num_growable_items) {
            debug<<"iteration ...";

            int available_space_per_item = available_space / num_growable_items;

            printvar(available_space);
            printvar(available_space_per_item);

            if (!available_space_per_item)
                break;

            // determine minimum current size among growable items
            int min_size = 0;
            for (int i = 0; i < num_children; i++) {
                LayoutItem &item = layout_items[i];
                if (item.canGrow()) {
                    if (!min_size || min_size > item.size)
                        min_size = item.size;
                }
            }

            int new_min_size = min_size + available_space_per_item;
            printvar(new_min_size);

            // grow items to new_min_size
            for (int i = 0; i < num_children; i++) {
                LayoutItem &item = layout_items[i];

                debug<<"item"<<i<<"size ="<<item.size;

                if (item.canGrow()) {
                    int delta;
                    if (item.max_size && item.max_size < new_min_size)
                        delta = item.max_size - item.size;
                    else
                        delta = new_min_size - item.size;

                    if (delta > 0) { // item is smaller than new_min_size / item.max_size
                        // grow item to either new_min_size or item.max_size
                        available_space -= delta;
                        item.size += delta;
                    }
                    if (!item.canGrow())
                        num_growable_items--;
                }
                debug<<"item"<<i<<"size ="<<item.size;
            }

            debug<<"available space after:"<<available_space;
        }
        // TODO: don't waste any available space: if (available_space) { ... }
    }

    assert(available_space >= 0);

    int current_x = client_rect.x;
    int current_y = client_rect.y;

    for(int i = 0; i < num_children; i++) {
        LayoutItem &item = layout_items[i];
        Container *c = item.container;

        Rect child_rect;
        child_rect.setPos(current_x, current_y);

        int size = item.size;

        debug<<"child"<<i<<"final size:"<<size;

        if (container->workspace()->maximized() && container->isActive()) {
            if (container->activeChild() == c)
                size += available_space;
        }

        if (is_horizontal) {
            child_rect.w = size;
            child_rect.h = client_rect.h;

            current_x += child_rect.w;
        } else {
            child_rect.w = client_rect.w;
            child_rect.h = size;

            current_y += child_rect.h;
        }

        Rect new_rect = child_rect;
        new_rect.x += ContainerContainer::_child_frame_width;
        new_rect.y += ContainerContainer::_child_frame_width;
        new_rect.w -= (2 * ContainerContainer::_child_frame_width);
        new_rect.h -= (2 * ContainerContainer::_child_frame_width);

        debug<<"child"<<i<<"final width:"<<new_rect.w;

        c->setRect(new_rect);
        c->layout();
   }

   delete[] layout_items;
}
