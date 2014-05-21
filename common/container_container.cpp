#include "container_container.h"

#include "client_container.h"
#include "container_container_layout.h"
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
//     _dirty(true)
//     _reserved_space(0)
    _layout(new ContainerContainerLayout(this))
{
}

ContainerContainer::~ContainerContainer()
{
    delete _layout;
    _layout = 0;
}

ContainerLayout *ContainerContainer::getLayout()
{
    return _layout;
}

#if 0
void ContainerContainer::setFocus()
{
    if (_active_child)
        _active_child->setFocus();
    redrawAll();
}
#endif

void ContainerContainer::handleMaximizedChanged()
{
    for (int i = 0; i < numElements(); i++)
        child(i)->handleMaximizedChanged();
}

void ContainerContainer::handleActiveChanged()
{
    if (activeChild())
        activeChild()->handleActiveChanged();
}

void ContainerContainer::handleSizeHintsChanged(Container *child)
{
    if (parent())
        parent()->handleSizeHintsChanged(this);
    else
        getLayout()->layoutContents();
}

ClientContainer *ContainerContainer::activeClientContainer()
{
    if (Container *active = activeChild()) {
        return active->activeClientContainer();
    } else
        return 0;
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

#if 0
void ContainerContainer::layout()
{
    Layout::layoutContainer(this);
    redraw();
}
#endif


#if 0
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
#endif
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
int ContainerContainer::hierarchyDepth()
{
    if (_parent)
        return _parent->hierarchyDepth() + 1;
    else
        return 0;
}
#endif


void ContainerContainer::redrawAll()
{
    redraw();
    for(int i = 0; i < numElements(); i++)
        child(i)->redrawAll();
}


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
