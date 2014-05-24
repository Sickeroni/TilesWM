#include "container_container.h"

#include "client_container.h"
#include "container_container_layout.h"
#include "canvas.h"
#include "colors.h"
#include "workspace.h"
#include "common.h"
#include "theme.h"

#include <sstream>
#include <stdlib.h>


ContainerContainer::ContainerContainer() : Container(CONTAINER),
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

void ContainerContainer::handleMaximizedChanged()
{
    for(int i = 0; i < numElements(); i++)
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

void ContainerContainer::redrawAll()
{
    redraw();
    for(int i = 0; i < numElements(); i++)
        child(i)->redrawAll();
}

void ContainerContainer::deleteEmptyChildren()
{
//     if (!_dirty)
//         return;

    // 1st pass: recurse
    for (int i = 0; i < numElements(); i++) {
        if (child(i)->isContainerContainer())
            static_cast<ContainerContainer*>(child(i))->deleteEmptyChildren();
    }

    // 2nd pass: dissolve child containers with only one child
    for (int i = 0; i < numElements(); i++) {
        if (child(i)->isContainerContainer() && (child(i)->numElements() == 1)) {
            Container *c =  static_cast<ContainerContainer*>(child(i))->removeChild(0);

            delete replaceChild(i, c);
        }
    }

    // 3rd pass: delete all empty children
    for (int i = 0; i < numElements(); ) {
        if (child(i)->isEmpty())
            delete removeChild(i);
        else
            i++;
    }

//     _dirty = isEmpty();

    getLayout()->layoutContents();
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
