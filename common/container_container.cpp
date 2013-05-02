#include "container_container.h"

#include "client_container.h"
#include "canvas.h"

#if 1

#include <iostream>
#include <stdlib.h>


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

void ContainerContainer::focusPrevChild()
{
    if (_active_child && _active_child->prev())
        _active_child = _active_child->prev();
    setFocus(); //FIXME HACK
    redrawAll();
}

void ContainerContainer::focusNextChild()
{
    if (_active_child && _active_child->next())
        _active_child = _active_child->next();
    setFocus(); //FIXME HACK
    redrawAll();
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
    rect.set(frame_width, frame_width + title_height,
             _rect.w - (frame_width * 2), _rect.h - title_height - (frame_width * 2));
}

void ContainerContainer::draw(Canvas *canvas)
{
    Rect bg_rect = _rect;
    bg_rect.setPos(0, 0);
    canvas->erase(bg_rect);

    Rect client_rect;
    getClientRect(client_rect);

    if (!_children.count() || !client_rect.w || !client_rect.h)
        return;

    int cell_width = 0, cell_height = 0;

    if (isHorizontal()) {
        cell_width = client_rect.w / _children.count();
        cell_height = client_rect.h;
    } else {
        cell_width = client_rect.w;
        cell_height = client_rect.h / _children.count();
    }

    Rect frame_rect;
    frame_rect.setSize(cell_width, cell_height);

    frame_rect.w -= 10;
    frame_rect.h -= 10;

    int i = 0;
    for(Container *c = _children.first(); c; c = c->next()) {
        if (isHorizontal()) {
            frame_rect.x = client_rect.x + (i * cell_width);
            frame_rect.y = client_rect.y;
        } else {
            frame_rect.x = client_rect.x;
            frame_rect.y = client_rect.y + (i * cell_height);
        }

        frame_rect.x += 5;
        frame_rect.y += 5;


        canvas->drawFrame(frame_rect, 0);

//         if (hasFocus() && activeChild() == c) {
        if (c->hasFocus()) {
            Rect focus_rect = frame_rect;
            focus_rect.x += 2;
            focus_rect.y += 2;
            focus_rect.w -= 4;
            focus_rect.h -= 4;

            unsigned long color = c->isClientContainer() ? 0xFF5555 : 0xFFBB00;

            canvas->drawFrame(focus_rect, color);
        }

        i++;
   }
}

void ContainerContainer::layout()
{
    if (!width() || !height() || !_children.count())
        return;

    Rect client_rect;
    getClientRect(client_rect);

    if (!client_rect.w || !client_rect.h)
        return;

    std::cout<<"ContainerContainer::layout()\n";
    std::cout<<"children: "<<_children.count()<<'\n';

    int cell_width = 0, cell_height = 0;

    if (isHorizontal()) {
        cell_width = client_rect.w / _children.count();
        cell_height = client_rect.h;
    } else {
        cell_width = client_rect.w;
        cell_height = client_rect.h / _children.count();
    }

    std::cout<<"cell_width: "<<cell_width<<"\n";
    std::cout<<"cell_height: "<<cell_height<<"\n";
    std::cout<<"=================================\n";

    Rect new_rect;
    new_rect.setSize(cell_width, cell_height);

    new_rect.w -= 2*child_frame_width;
    new_rect.h -= 2*child_frame_width;

    int i = 0;
    for(Container *c = _children.first(); c; c = c->next()) {
        if (isHorizontal()) {
            new_rect.x = client_rect.x + (i * cell_width);
            new_rect.y = client_rect.y;
        } else {
            new_rect.x = client_rect.x;
            new_rect.y = client_rect.y + (i * cell_height);
        }

        new_rect.x += child_frame_width;
        new_rect.y += child_frame_width;

//         localToGlobal(x, y);

        c->setRect(new_rect);
        c->layout();

        i++;
   }

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

#if 1
void ContainerContainer::deleteEmptyChildren()
{
    //FIXME - _active_child

//     if (!_dirty)
//         return;

    for (Container *child = _children.first(); child; ) {
        Container *delete_this = 0;

        if (child->isEmpty())
            delete_this = child;
        else if (child->isContainerContainer()) {
            ContainerContainer *c = static_cast<ContainerContainer*>(child);
//             if (c->_dirty)
                c->deleteEmptyChildren();
            if (c->isEmpty()) {
                delete_this = child;
            }
        }

        child = child->next();

        if (delete_this)
            deleteChild(delete_this);
    }

    if (_children.count() == 1 && !_children.first()->isClientContainer()) {
        //FIXME add function dissolveChild()

        // reparent child containers
        ContainerContainer *child = static_cast<ContainerContainer*>(_children.first());
        child->_active_child = 0;

        Container *c = child->_children.first();
        while (c) {
            Container *reparent_this = c;
            c = c->next();

            child->_children.remove(reparent_this);
            reparent_this->reparent(this);
            appendChild(reparent_this);
        }
        deleteChild(child);
    }

    _dirty = isEmpty();

    if (_dirty && !_parent) { // TODO - replace this with empty ClientContainer ?

    }

    layout();
}
#endif


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


void ContainerContainer::setActiveChild(Container *child)
{
    assert(child->parent() == this);
    _active_child = child;
    _active_child->setFocus(); //FIXME
}

#if 1
ClientContainer *ContainerContainer::splitChild(Container *child, bool prepend_new_silbling)
{
    if (hierarchyDepth() >= max_hierarchy_depth)
        return 0;

    // create new parent
    ContainerContainer *new_parent = createContainerContainer();

    replaceChild(child, new_parent); // unlinks container

    child->reparent(new_parent);
    new_parent->appendChild(child);

    ClientContainer *new_silbling = new_parent->createClientContainer();

    // add this + new child container to new parent
    if (prepend_new_silbling)
        new_parent->prependChild(new_silbling);
    else
        new_parent->appendChild(new_silbling);

    _active_child = new_parent;
    new_parent->_active_child = new_silbling;

    layout();

    return new_silbling;
}
#endif


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
