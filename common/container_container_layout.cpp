#include "container_container_layout.h"

#include "theme.h"
#include "container_container.h"
#include "common.h"
#include "workspace.h"

ContainerContainerLayout::ContainerContainerLayout(ContainerContainer *container) :
    _container(container)
{
}

int ContainerContainerLayout::minWidth()
{
    const Theme::ContainerContainerSizes &sizes = Theme::containerContainerSizes();

    int width = 0;
    if (_container->isHorizontal()) {
        for (int i = 0; i < _container->numElements(); i++)
            width += _container->child(i)->getLayout()->minWidth() + (2 * sizes.child_frame_width);
    } else {
        for (int i = 0; i < _container->numElements(); i++) {
            int w = _container->child(i)->getLayout()->minWidth();
            if (w > width)
                width = w;
        }
        width += (2 * sizes.child_frame_width);
    }

    width += 2 * sizes.frame_width;

    return width;
}

int ContainerContainerLayout::minHeight()
{
    const Theme::ContainerContainerSizes &sizes = Theme::containerContainerSizes();

    int height = 0;
    if (_container->isHorizontal()) {
        for (int i = 0; i < _container->numElements(); i++) {
            int h = _container->child(i)->getLayout()->minHeight();
            if (h > height)
                height = h;
        }
        height += (2 * sizes.child_frame_width);
    } else {
        for (int i = 0; i < _container->numElements(); i++)
            height += _container->child(i)->getLayout()->minHeight() + (2 * sizes.child_frame_width);
    }

    height += sizes.title_height + (2 * sizes.frame_width);

    return height;
}

int ContainerContainerLayout::maxWidth()
{
    const Theme::ContainerContainerSizes &sizes = Theme::containerContainerSizes();

    int max_width = 0;
    if (_container->isHorizontal()) {
        abort();
    } else {
        for(int i = 0; i < _container->numElements(); i++) {
            if (int w = _container->child(i)->getLayout()->maxWidth()) {
                if (w > max_width)
                    max_width = w;
            } else { // at least one child has unlimited maximum width
                max_width = 0; // unlimited width
                break;
            }
        }
    }
    return max_width;
}

int ContainerContainerLayout::maxHeight()
{
    //FIXME
    return 0;
}

void ContainerContainerLayout::layoutContents()
{
    static const bool respect_size_hints = true;

    const Theme::ContainerContainerSizes &sizes = Theme::containerContainerSizes();

    struct LayoutItem
    {
        void init(Container *c, int min, int max) {
            const Theme::ContainerContainerSizes &sizes = Theme::containerContainerSizes();

            container = c;
            min_size = min;
            max_size = max;

            if (max_size && max_size < min_size) // normalize
                max_size = min_size;

            min_size += (2 * sizes.child_frame_width);
            if (max_size)
                max_size += (2 * sizes.child_frame_width);

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


    const int num_children = _container->numElements();
    const bool is_horizontal = _container->isHorizontal();

    if (!_container->width() || !_container->height() || !num_children)
        return;

    Rect client_rect;
    Theme::getContainerContainerClientRect(_container->rect(), client_rect);

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
            Container *c = _container->child(i);

            int min_size, max_size;
            if (!respect_size_hints) // FIXME move this flag to Container and test against in it Container::minSize()/maxSize()
                min_size = max_size = 0;
            else if (is_horizontal) {
                min_size = c->getLayout()->minWidth();
                max_size = c->getLayout()->maxWidth();
            } else {
                min_size = c->getLayout()->minHeight();
                max_size = c->getLayout()->maxHeight();
            }

            if (c->isFixedSize()) {
                //FIXME - make sure min_size <= c->[maxWidth()|maxHeight()]
                if (is_horizontal) {
                    min_size = c->getLayout()->minWidth() + c->fixedWidth();
                } else {
                    min_size = c->getLayout()->minHeight() + c->fixedHeight();
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

    if (!(_container->workspace()->maximized())) {
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

        if (_container->workspace()->maximized() && _container->isActive()) {
            if (_container->activeChild() == c)
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
        new_rect.x += sizes.child_frame_width;
        new_rect.y += sizes.child_frame_width;
        new_rect.w -= (2 * sizes.child_frame_width);
        new_rect.h -= (2 * sizes.child_frame_width);

        debug<<"child"<<i<<"final width:"<<new_rect.w;

        c->setRect(new_rect);
        c->getLayout()->layoutContents();
   }

   delete[] layout_items;

   _container->redraw();
}
