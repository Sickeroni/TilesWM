#include "container_container.h"

#include "client_container.h"
#include "canvas.h"
#include "colors.h"
#include "workspace.h"
#include "common.h"
#include "layout.h"
#include "theme.h"

#include <sstream>

#if 1

#include <stdlib.h>


ContainerContainer::ContainerContainer(ContainerContainer *parent) : Container(CONTAINER, parent),
//     _active_child(0),
    _dirty(true),
    _reserved_space(0)
{
}

ContainerContainer::~ContainerContainer()
{
    clear();
}

#if 0
void ContainerContainer::clear()
{
    for (Container *c = _children.first(); c; ) {
        Container *delete_this= c;
        c = c->next();
        deleteChild(delete_this);
    }
}
#endif

#if 0
void ContainerContainer::setFocus()
{
    if (_active_child)
        _active_child->setFocus();
    redrawAll();
}
#endif

#if 0
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
#endif

void ContainerContainer::handleSizeHintsChanged(Container *child)
{
    if (parent())
        parent()->handleSizeHintsChanged(this);
    else
        layout();
}

#if 0
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
#endif

#if 0
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
#endif

ClientContainer *ContainerContainer::activeClientContainer()
{
    if (Container *active = activeChild()) {
        return active->activeClientContainer();
    } else
        return 0;
}

#if 0
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
#endif

inline void ContainerContainer::getClientRect(Rect &rect)
{
    rect.set(_frame_width, _frame_width + _title_height,
             _rect.w - (_frame_width * 2), _rect.h - _title_height - (_frame_width * 2));
}

#if 0
int ContainerContainer::minWidthInt()
{
    int width = 0;
    if (isHorizontal()) {
        for (Container *c = _children.first(); c; c = c->next())
            width += c->minWidth() + (2 * _child_frame_width);
    } else {
        for (Container *c = _children.first(); c; c = c->next()) {
            int w = c->minWidth();
            if (w > width)
                width = w;
        }
        width += (2 * _child_frame_width);
    }

    width += 2 * _frame_width;

    return width;
}

int ContainerContainer::minHeightInt()
{
    int height = 0;
    if (isHorizontal()) {
        for (Container *c = _children.first(); c; c = c->next()) {
            int h = c->minHeight();
            if (h > height)
                height = h;
        }
        height += (2 * _child_frame_width);
    } else {
        for (Container *c = _children.first(); c; c = c->next())
            height += c->minHeight() + (2 * _child_frame_width);
    }

    height += _title_height + (2 * _frame_width);

    return height;
}

int ContainerContainer::maxWidthInt()
{
    int max_width = 0;
    if (isHorizontal()) {
        abort();
    } else {
        for(Container *c = _children.first(); c; c = c->next()) {
            if (int w = c->maxWidth()) {
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

int ContainerContainer::maxHeightInt()
{
    return 0;
}
#endif

void ContainerContainer::draw(Canvas *canvas)
{
    Theme::drawContainerContainer(this, canvas);
}

#if 0
int ContainerContainer::calcAvailableSpace()
{
    //FIXME duplicated in layout()
    Rect client_rect;
    getClientRect(client_rect);

    int available_space = isHorizontal() ? client_rect.w : client_rect.h;

    for(Container *c = _children.first(); c; c = c->next())
        available_space -= isHorizontal() ? c->minWidth() : c->minHeight();

    if (available_space < 0)
        available_space = 0;

    return available_space;
}
#endif

void ContainerContainer::layout()
{
    Layout::layoutContainer(this);
    redraw();
}

#if 0
ClientContainer *ContainerContainer::addNewClientContainer(bool prepend)
{
    debug;
    ClientContainer *client_container = createClientContainer();
    if (prepend)
        prependChild(client_container);
    else
        appendChild(client_container);

    _active_child = client_container;

    layout();

    return client_container;
}
#endif

#if 0
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
    printvar(_children.count());

//FIXME
//     if (!container->isUnlinked())
//         abort();

    _children.append(container);

    printvar(_children.count());

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
#endif

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
#if 0
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
#endif
#if 0
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
#endif

#if 0
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
#endif

#if 0
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
#endif

int ContainerContainer::hierarchyDepth()
{
    if (_parent)
        return _parent->hierarchyDepth() + 1;
    else
        return 0;
}

#if 0
void ContainerContainer::redrawAll()
{
    redraw();
    for (Container *c = _children.first(); c; c = c->next())
        c->redrawAll();
}
#endif

// void ContainerContainer::incAvailableSpacePortion(Container *child, int pixels)
// {
//     printvar(pixels);
//     if (pixels <= 0)
//         return;
//     if (_reserved_space < 1) {
//         int available_space = calcAvailableSpace();
//         printvar(available_space);
//         double portion_delta = static_cast<double>(pixels) / static_cast<double>(available_space);
//         printvar(portion_delta);
//         printvar(_reserved_space);
//         printvar(_reserved_space + portion_delta);
//         if (_reserved_space + portion_delta > 1)
//             portion_delta = (1 - _reserved_space);
//         printvar(portion_delta);
//         _reserved_space += portion_delta;
//         printvar(_reserved_space);
//         child->setAvailableSpacePortion(child->availableSpacePortion() + portion_delta);
//         layout();
//     }
// }
// 
// void ContainerContainer::decAvailableSpacePortion(Container *child, int pixels)
// {
//     printvar(pixels);
//     if (pixels <= 0)
//         return;
//     if (_reserved_space > 0 && child->availableSpacePortion() > 0) {
//         int available_space = calcAvailableSpace();
//         printvar(available_space);
//         double portion_delta = static_cast<double>(pixels) / static_cast<double>(available_space);
//         printvar(portion_delta);
//         printvar(_reserved_space);
//         printvar(_reserved_space - portion_delta);
// //         if (_reserved_space - portion_delta < 0)
// //             portion_delta -= _reserved_space;
//         printvar(portion_delta);
//         _reserved_space -= portion_delta;
//         printvar(_reserved_space);
//         child->setAvailableSpacePortion(child->availableSpacePortion() - portion_delta);
//         layout();
//     }
// }


#endif
