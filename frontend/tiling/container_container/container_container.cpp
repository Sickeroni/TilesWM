#include "container_container.h"

#include "client_container.h"
#include "container_container_layout.h"
#include "container_container_theme.h"
#include "workspace.h"
#include "common.h"

#include <sstream>
#include <stdlib.h>


ContainerContainer::ContainerContainer() : Container(OTHER),
//     _dirty(true)
    _layout(new ContainerContainerLayout(this))
{
}

ContainerContainer::~ContainerContainer()
{
    clear();
    delete _layout;
    _layout = 0;
}

void ContainerContainer::clear()
{
    _active_child_index = INVALID_INDEX;

    for(Container *child : _children)
        delete child;
    _children.clear();
}

int ContainerContainer::indexOfChild(const Container *child)
{
    for (int i = 0; i < numElements(); i++) {
        if (child == _children[i])
            return i;
    }
    assert(false);
    abort();
}

int ContainerContainer::addChild(Container *child)
{
    assert(!child->parent());

    child->setOrientation(rotatedOrientation(orientation()));

    child->reparent(this, _backend);

    _children.push_back(child);
    printvar(numElements());

    applyMinimizeMode(child);
    child->setMapped(true);

    getLayout()->layoutContents();

    return numElements() - 1;
}

void ContainerContainer::insertChild(Container *child, int insert_pos)
{
    assert(!child->parent());

    child->setOrientation(rotatedOrientation(orientation()));

    child->reparent(this, _backend);

    assert(insert_pos <= numElements());
    _children.insert(_children.begin() + insert_pos, child);

    applyMinimizeMode(child);
    child->setMapped(true);

    getLayout()->layoutContents();
}

Container *ContainerContainer::replaceChild(int index, Container *new_child)
{
    assert(!new_child->parent());
    assert(index < numElements());

    new_child->setOrientation(rotatedOrientation(orientation()));

    Container *old_child = _children[index];

    old_child->reparent(0, 0);
    new_child->reparent(this, _backend);

    _children[index] = new_child;

    applyMinimizeMode(new_child);
    new_child->setMapped(true);

    getLayout()->layoutContents();

    return old_child;
}

Container *ContainerContainer::removeChild(int index)
{
    assert(index < numElements());

    Container *child = _children[index];

    child->reparent(0, 0);

    _children.erase(_children.begin() + index);

    if (_active_child_index >= numElements())
        _active_child_index = numElements() -1;

    return child;
}

void ContainerContainer::setActiveChild(int index)
{
    assert(index < numElements());

    if (index == _active_child_index)
        return;

    if (index < numElements()) {
        Container *old_active = 0;
        if (_active_child_index != INVALID_INDEX)
            old_active = _children[_active_child_index];

        _active_child_index = index;

        if (_minimize_mode == MINIMIZE_INACTIVE) {
            if (old_active)
                applyMinimizeMode(old_active);
            if (_active_child_index != INVALID_INDEX)
                applyMinimizeMode(_children[_active_child_index]);

            getLayout()->layoutContents();
        }
    }

    redraw();
}

ContainerLayout *ContainerContainer::getLayout()
{
    return _layout;
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
    for(Container *child : _children)
        child->redrawAll();
}

void ContainerContainer::setMinimizeMode(MinimizeMode mode)
{
    if (_minimize_mode == mode)
        return;

    Container *active_child = activeChild();

    switch (mode) {
        case MINIMIZE_NONE:
        case MINIMIZE_ALL:
            for(Container *child : _children) {
                if (child->isClientContainer())
                    child->toClientContainer()->setMinimized(mode == MINIMIZE_ALL);
                else
                    child->to<ContainerContainer>()->setMinimizeMode(mode);
            }
            break;
        case MINIMIZE_INACTIVE:
            for(Container *child : _children) {
                if (child->isClientContainer())
                    child->toClientContainer()->setMinimized(active_child != child);
                else
                    child->to<ContainerContainer>()->setMinimizeMode(
                        (active_child == child) ? MINIMIZE_INACTIVE : MINIMIZE_ALL);
            }
            break;
    }

    _minimize_mode = mode;
}

void ContainerContainer::applyMinimizeMode(Container *child)
{
    bool is_active = (activeChild() == child);

    switch (_minimize_mode) {
        case MINIMIZE_NONE:
        case MINIMIZE_ALL:
            if (child->isClientContainer())
                child->toClientContainer()->setMinimized(_minimize_mode == MINIMIZE_ALL);
            else
                child->to<ContainerContainer>()->setMinimizeMode(_minimize_mode);
            break;
        case MINIMIZE_INACTIVE:
            if (child->isClientContainer())
                child->toClientContainer()->setMinimized(!is_active);
            else
                child->to<ContainerContainer>()->setMinimizeMode(
                    is_active ? MINIMIZE_INACTIVE : MINIMIZE_ALL);

            break;
    }

}

void ContainerContainer::draw(Canvas *canvas)
{
    Theme::drawContainerContainer(this, canvas);
}

void ContainerContainer::setOrientation(Orientation o)
{
    Container::setOrientation(o);
    for (Container *child : _children)
        child->setOrientation(rotatedOrientation(orientation()));
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
