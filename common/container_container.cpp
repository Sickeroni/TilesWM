#include "container_container.h"

#include "client_container.h"
#include "canvas.h"
#include "colors.h"
#include "workspace.h"

#if 1

#include <iostream>
#include <stdlib.h>


using std::cout;
using std::endl;


ContainerContainer::ContainerContainer(ContainerContainer *parent) : Container(CONTAINER, parent),
    _active_child(0),
    _dirty(true)
{
}

ContainerContainer::~ContainerContainer()
{
    clear();
}


void ContainerContainer::clear()
{
    for (Container *c = _children.first(); c; ) {
        Container *delete_this= c;
        c = c->next();
        deleteChild(delete_this);
    }
}

void ContainerContainer::setFocus()
{
    if (_active_child)
        _active_child->setFocus();
    redrawAll();
}

void ContainerContainer::handleMaximizedChanged()
{
    for (Container *c = _children.first(); c; c = c->next())
        c->handleMaximizedChanged();
}

void ContainerContainer::handleActiveChanged()
{
    if (_active_child)
        _active_child->handleActiveChanged();
}

void ContainerContainer::handleSizeHintsChanged(Container *child)
{
    if (parent())
        parent()->handleSizeHintsChanged(this);
    else
        layout();
}


void ContainerContainer::setActiveChild(Container *child)
{
    assert(child->parent() == this);

    if (_active_child == child)
        return;

    Container *old_active = _active_child;
    _active_child = child;

    old_active->handleActiveChanged();
    _active_child->handleActiveChanged();

    if (workspace()->maximized() && hasFocus())
        layout();

#if 0
    if (old_active)
        old_active->handleActiveChanged();
    if (_active_child)
        _active_child->handleActiveChanged();

    if (workspace()->maximized() && hasFocus()) {
        _active_child->handleMaximizedChanged();
        _active_child = child;
        _active_child->handleMaximizedChanged();
        layout();
    } else
        _active_child = child;
#endif
    _active_child->setFocus(); //FIXME

    redrawAll();
}

void ContainerContainer::focusPrevChild()
{
    if (_active_child && _active_child->prev())
        setActiveChild(_active_child->prev());
}

void ContainerContainer::focusNextChild()
{
    if (_active_child && _active_child->next())
        setActiveChild(_active_child->next());
}

ClientContainer *ContainerContainer::activeClientContainer()
{
    if (Container *active = activeChild()) {
        return active->activeClientContainer();
    } else
        return 0;
}


void ContainerContainer::addClient(Client *c)
{
    if(activeChild())
        activeChild()->addClient(c);
    else {
        ClientContainer *client_container = createClientContainer();
        appendChild(client_container);
        client_container->addClient(c);
    }
}

inline void ContainerContainer::getClientRect(Rect &rect)
{
    rect.set(_frame_width, _frame_width + _title_height,
             _rect.w - (_frame_width * 2), _rect.h - _title_height - (_frame_width * 2));
}

int ContainerContainer::minimumWidth()
{
    int width = 0;
    if (isHorizontal()) {
        for (Container *c = _children.first(); c; c = c->next())
            width += c->minimumWidth() + (2 * _child_frame_width);
    } else {
        for (Container *c = _children.first(); c; c = c->next()) {
            int w = c->minimumWidth();
            if (w > width)
                width = w;
        }
        width += (2 * _child_frame_width);
    }

    width += 2 * _frame_width;

    return width;
}

int ContainerContainer::minimumHeight()
{
    int height = 0;
    if (isHorizontal()) {
        for (Container *c = _children.first(); c; c = c->next()) {
            int h = c->minimumHeight();
            if (h > height)
                height = h;
        }
        height += (2 * _child_frame_width);
    } else {
        for (Container *c = _children.first(); c; c = c->next())
            height += c->minimumHeight() + (2 * _child_frame_width);
    }

    height += _title_height + (2 * _frame_width);

    return height;
}

int ContainerContainer::maximumWidth()
{
    int max_width = 100;
    for(Container *c = _children.first(); c; c = c->next()) {
        if (int w = c->maximumWidth()) {
            if (w > max_width)
                max_width = w;
        } else { // at least one child has unlimited maximum width
            max_width = 0; // unlimited width
            break;
        }
    }
    return max_width;
}

int ContainerContainer::maximumHeight()
{
    return 0;
}

void ContainerContainer::draw(Canvas *canvas)
{
    Rect bg_rect = _rect;
    bg_rect.setPos(0, 0);
    canvas->erase(bg_rect);

    Rect title_rect(_frame_width - 2, _frame_width - 2,
                    width() - 4 - (2 * _frame_width), _title_height);

    canvas->drawText(isHorizontal() ? "H" : "V",
                     title_rect,
                     hasFocus() ?  Colors::CONTAINER_FOCUS : Colors::CONTAINER_FRAME);

    for (Container *c = _children.first(); c; c = c->next()) {
        Rect frame_rect = c->rect();
        frame_rect.x -= 5;
        frame_rect.y -= 5;
        frame_rect.w += 10;
        frame_rect.h += 10;

        uint32 frame_color = c->isClientContainer() ? Colors::CLIENT_CONTAINER_FRAME :
                                                      Colors::CONTAINER_FRAME;

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

void ContainerContainer::layout()
{
    static const bool respect_size_hints = true;

    struct LayoutItem
    {
        void init(int min_size, int max_size) {
            if (respect_size_hints) {
                this->min_size = min_size;
                this->max_size = max_size;
            } else
                this->max_size = this->min_size = 0;

            if (max_size && max_size < min_size) // normalize
                max_size = min_size;

            this->min_size += (2 * _child_frame_width);
            if (this->max_size)
                this->max_size += (2 * _child_frame_width);

            size = this->min_size;
        }

        bool canGrow() {
            return !max_size || size < max_size;
        }

        int size;
        int min_size, max_size;
    };


    if (!width() || !height() || !_children.count())
        return;

    Rect client_rect;
    getClientRect(client_rect);

    if (!client_rect.w || !client_rect.h)
        return;

    std::cout<<"ContainerContainer::layout()\n";
    std::cout<<"children: "<<_children.count()<<'\n';

    int available_space = isHorizontal() ? client_rect.w : client_rect.h;

    cout<<"space: "<<available_space<<endl;


    // create layout item for each child
    LayoutItem *layout_items = new LayoutItem[_children.count()];


    int num_growable_children = _children.count();

    // initialize layout items
    int i = 0;
    for (Container *c = _children.first(); c; c = c->next()) {
        LayoutItem &item = layout_items[i];
        if (isHorizontal())
            item.init(c->minimumWidth(), c->maximumWidth());
        else
            item.init(c->minimumHeight(), c->maximumHeight());

        cout<<"item "<<i<<" size: "<<item.size<<endl;
        cout<<"item "<<i<<" min size: "<<item.min_size<<endl;
        cout<<"item "<<i<<" max size: "<<item.max_size<<endl;

        available_space -= item.size;

        if(!item.canGrow())
            num_growable_children--;

        i++;
    }

    if (available_space < 0) // BAAD - children won't fit
        available_space = 0;

    cout<<"space after - minimum size: "<<available_space<<endl;

    if (!(workspace()->maximized())) {
        // distribute remaining available space
        while (available_space && num_growable_children) {
            int available_space_per_child = available_space / num_growable_children;
            cout<<"available space before: "<<available_space<<endl;
            cout<<"available space per child: "<<available_space_per_child<<endl;

            if (!available_space_per_child)
                break;

            for (int i = 0; i < _children.count(); i++) {
                LayoutItem &item = layout_items[i];
                cout<<"i: "<<i<<endl;
                cout<<"item size before: "<<item.size<<endl;

                if (item.canGrow()) {
                    if (item.max_size) {
                        if (item.max_size < (item.size + available_space_per_child)) {
                            available_space -= (item.max_size - item.size);
                            item.size = item.max_size;
                        } else {
                            available_space -= available_space_per_child;
                            item.size += available_space_per_child;
                        }
                    } else {
                        available_space -= available_space_per_child;
                        item.size += available_space_per_child;
                    }

                    if (!item.canGrow())
                        num_growable_children--;
                }

                cout<<"item size after: "<<item.size<<endl;
            }
            cout<<"available space after: "<<available_space<<endl;
        }
    }

    assert(available_space >= 0);

    int current_x = client_rect.x;
    int current_y = client_rect.y;

    i = 0;
    for(Container *c = _children.first(); c; c = c->next()) {
        LayoutItem &item = layout_items[i];

        Rect child_rect;
        child_rect.setPos(current_x, current_y);

        int size = item.size;

        cout<<"child "<<i<<" final size: "<<size<<endl;

        if (workspace()->maximized() && hasFocus()) {
            if (activeChild() == c)
                size += available_space;
        }

        if (isHorizontal()) {
            child_rect.w = size;
            child_rect.h = client_rect.h;

            current_x += child_rect.w;
        } else {
            child_rect.w = client_rect.w;
            child_rect.h = size;

            current_y += child_rect.h;
        }

        Rect new_rect = child_rect;
        new_rect.x += _child_frame_width;
        new_rect.y += _child_frame_width;
        new_rect.w -= (2 * _child_frame_width);
        new_rect.h -= (2 * _child_frame_width);

        cout<<"child "<<i<<" final width: "<<new_rect.w<<endl;

        c->setRect(new_rect);
        c->layout();

        i++;
   }

   delete[] layout_items;

   redraw();
}

ClientContainer *ContainerContainer::addNewClientContainer(bool prepend)
{
    std::cout<<"ContainerContainer::addNewClientContainer()\n";
    ClientContainer *client_container = createClientContainer();
    if (prepend)
        prependChild(client_container);
    else
        appendChild(client_container);

    _active_child = client_container;

    layout();

    return client_container;
}

void ContainerContainer::prependChild(Container *container)
{
//FIXME
//     if (!container->isUnlinked())
//         abort();

    _children.prepend(container);

    if (!_active_child)
        _active_child = container;

    layout();
}

void ContainerContainer::appendChild(Container *container)
{
    std::cout<<"ContainerContainer::appendChild()\n";
    std::cout<<"children: "<<_children.count()<<'\n';

//FIXME
//     if (!container->isUnlinked())
//         abort();

    _children.append(container);

    std::cout<<"children: "<<_children.count()<<'\n';

    if (!_active_child)
        _active_child = container;

    if (parent())
        parent()->handleSizeHintsChanged(this);

    layout();
}

void ContainerContainer::replaceChild(Container *old_container, Container *new_container)
{
//FIXME
//     if (!new_container->isUnlinked())
//         abort();

    _children.replace(old_container, new_container);

    if (_active_child == old_container)
        _active_child = new_container;

//     updateDirtyStatus();

    layout();
}

void ContainerContainer::setDirty(bool set)
{
    _dirty = set;

    if (_parent) {
        if (_dirty)
            _parent->setDirty(true);
        else
            _parent->updateDirtyStatus();
    }
}

void ContainerContainer::updateDirtyStatus()
{
    //FIXME a bit ugly

    bool dirty = false;
    for(Container *c = _children.first(); c; c = c->next()) {
        if (c->isEmpty()) {
            dirty = true;
            break;
        } else if (c->isContainerContainer()) {
            if (static_cast<ContainerContainer*>(c)->_dirty) {
                dirty = true;
                break;
            }
        }
    }
    setDirty(dirty);
}

void ContainerContainer::deleteEmptyChildren()
{

//     if (!_dirty)
//         return;

    // 1st pass: recurse
    for (Container *child = _children.first(); child; child = child->next()) {
        if (child->isContainerContainer())
            static_cast<ContainerContainer*>(child)->deleteEmptyChildren();
    }

    // 2nd pass: dissolve child containers with only one child
    for (Container *child = _children.first(); child; ) {
        ContainerContainer *dissolve_this = 0;

        if (child->isContainerContainer() && (child->numElements() == 1))
            dissolve_this = static_cast<ContainerContainer*>(child);

        child = child->next();

        if (dissolve_this) {
            Container *c = dissolve_this->_children.first();
            dissolve_this->_active_child = 0;
            dissolve_this->_children.remove(c);

            c->reparent(this);

            _children.replace(dissolve_this, c);

            if (dissolve_this == _active_child)
                _active_child = c;

            delete dissolve_this;
            dissolve_this = 0;
        }
    }

    // 3rd pass: delete all empty children
    for (Container *child = _children.first(); child; ) {
        Container *delete_this = 0;

        if (child->isEmpty())
            delete_this = child;

        child = child->next();

        if (delete_this)
            deleteChild(delete_this);
    }

#if 0
    if (_children.count() == 1 && _children.first()->isContainerContainer()) {
        //FIXME add function dissolveChild()

        // reparent child containers
        ContainerContainer *child = static_cast<ContainerContainer*>(_children.first());

        _children.remove(child);
        _active_child = child->_active_child;
        child->_active_child = 0;

        Container *c = child->_children.first();
        while (c) {
            Container *reparent_this = c;
            c = c->next();

            child->_children.remove(reparent_this);
            reparent_this->reparent(this);
            _children.append(reparent_this);
        }

        delete child;
    }
#endif

    _dirty = isEmpty();

    layout();
}

void ContainerContainer::deleteChild(Container *child)
{
    if (child == _active_child) {
        _active_child = child->prev();
        if (!_active_child)
            _active_child = child->next();
    }

    _children.remove(child);

    delete child;
}

ClientContainer *ContainerContainer::splitChild(Container *child, bool prepend_new_sibling)
{
    if (hierarchyDepth() >= _max_hierarchy_depth)
        return 0;

    // create new parent
    ContainerContainer *new_parent = createContainerContainer();

    replaceChild(child, new_parent); // unlinks container

    child->reparent(new_parent);
    new_parent->appendChild(child);

    ClientContainer *new_sibling = new_parent->createClientContainer();

    // add this + new child container to new parent
    if (prepend_new_sibling)
        new_parent->prependChild(new_sibling);
    else
        new_parent->appendChild(new_sibling);

    _active_child = new_parent;
    new_parent->_active_child = new_sibling;

    layout();

    return new_sibling;
}

int ContainerContainer::hierarchyDepth()
{
    if (_parent)
        return _parent->hierarchyDepth() + 1;
    else
        return 0;
}

void ContainerContainer::redrawAll()
{
    redraw();
    for (Container *c = _children.first(); c; c = c->next())
        c->redrawAll();
}

#endif
