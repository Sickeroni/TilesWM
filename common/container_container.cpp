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
}

void ContainerContainer::focusPrevChild()
{
    if (_active_child && _active_child->prev())
        _active_child = _active_child->prev();
    setFocus(); //FIXME HACK
    redraw();
}

void ContainerContainer::focusNextChild()
{
    if (_active_child && _active_child->next())
        _active_child = _active_child->next();
    setFocus(); //FIXME HACK
    redraw();
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
    canvas->erase(_rect);
    Rect r(_rect.x+5, _rect.y+5, _rect.w-10, _rect.h-10);
    canvas->drawFrame(r, 0);


    Rect client_rect;
    getClientRect(client_rect);

    if (!client_rect.w || !client_rect.h)
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

        if (activeChild() == c) {
            Rect focus_rect = frame_rect;
            focus_rect.x += 2;
            focus_rect.y += 2;
            focus_rect.w -= 4;
            focus_rect.h -= 4;
            canvas->drawFrame(focus_rect, 0xFFBB00);
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


void ContainerContainer::appendNewClientContainer()
{
    std::cout<<"ContainerContainer::appendNewClientContainer()\n";
    ClientContainer *client_container = createClientContainer();
    appendChild(client_container);
    layout();
}

#if 0
void ContainerContainer::prependChild(Container *container)
{
    if (container->parent())
        abort();
    if (!container->isUnlinked())
        abort();

    _children.prepend(container);
    container->setParent(this);

    layout();
}
#endif

void ContainerContainer::appendChild(Container *container)
{
    std::cout<<"ContainerContainer::appendChild()\n";
    std::cout<<"children: "<<_children.count()<<'\n';

    if (!container->isUnlinked())
        abort();

    _children.append(container);

    std::cout<<"children: "<<_children.count()<<'\n';

    if (!_active_child)
        _active_child = container;

    layout();
}


#if 0

void ContainerContainer::replaceChild(Container *old_container, Container *new_container)
{
    if (new_container->parent())
        abort();
    if (!new_container->isUnlinked())
        abort();

    _children.replace(old_container, new_container);

    //TODO if (_active_container == old_container)
    //          _active_container = new_container;

    updateDirtyStatus();

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

#if 0
void ContainerContainer::deleteEmptyChildren()
{
    //FIXME - _active_child

    if (!_dirty)
        return;

    for (Container *child = _children.first(); child; ) {
        Container *delete_this = 0;

        if (child->isEmpty())
            delete_this = child;
        else if (child->isContainerContainer()) {
            ContainerContainer *c = static_cast<ContainerContainer*>(child);
            if (c->_dirty)
                c->deleteEmptyChildren();
            if (c->isEmpty()) {
                delete_this = child;
            }
        }

        child = child->next();

        deleteChild(delete_this);
    }

    _dirty = isEmpty();

    if (_dirty && !_parent) { // TODO - replace this with empty ClientContainer ?

    }
}
#endif


void ContainerContainer::deleteChild(Container *child)
{
    //TODO if (_active_child)

    _children.remove(child);

    delete child;
}

#if 0
ClientContainer *ContainerContainer::splitChild(Container *child, bool prepend_new_silbling)
{
    // create new parent
    ContainerContainer *new_parent = createContainerContainer();

    replaceChild(child, new_parent); // unlinks container

    child->reparent(new_parent);
    new_parent->appendChild(new_silbling);

    ClientContainer *new_silbling = new_parent->createClientContainer();

    // add this + new child container to new parent
    if (prepend_new_silbling)
        new_parent->preendChild(new_silbling);
    else
        new_parent->appendChild(new_silbling);

    return new_silbling;
}
#endif


#endif
