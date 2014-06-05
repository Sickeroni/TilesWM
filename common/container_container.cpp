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
