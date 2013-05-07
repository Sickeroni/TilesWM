#include "container_container.h"

#include "client_container.h"
#include "canvas.h"
#include "workspace.h"

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
    for (Container *c = _children.first(); c; c = c->next())
        width += c->minimumWidth() + (2 * _child_frame_width);
    return width;
}

int ContainerContainer::minimumHeight()
{
    int height = 0;
    for (Container *c = _children.first(); c; c = c->next())
        height += c->minimumHeight() + (2 * _child_frame_width);
    return height;
}

void ContainerContainer::draw(Canvas *canvas)
{
    Rect bg_rect = _rect;
    bg_rect.setPos(0, 0);
    canvas->erase(bg_rect);

    for (Container *c = _children.first(); c; c = c->next()) {
        Rect frame_rect = c->rect();
        frame_rect.x -= 5;
        frame_rect.y -= 5;
        frame_rect.w += 10;
        frame_rect.h += 10;

        canvas->drawFrame(frame_rect, 0);

        if (c->hasFocus()) {
            Rect focus_rect = frame_rect;
            focus_rect.x += 2;
            focus_rect.y += 2;
            focus_rect.w -= 4;
            focus_rect.h -= 4;

            uint32 color = c->isClientContainer() ? 0xFF5555 : 0xFFBB00;

            canvas->drawFrame(focus_rect, color);
        }

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

    int available_space = isHorizontal() ? client_rect.w : client_rect.h;

    for (Container *c = _children.first(); c; c = c->next()) {
        if (isHorizontal())
            available_space -= c->minimumWidth() + (2 * _child_frame_width);
        else
            available_space -= c->minimumHeight() + (2 * _child_frame_width);
    }

    if (available_space < 0) // BAAD - children won't fit
        available_space = 0;

    int additional_space_per_child = available_space / _children.count();

    int current_x = client_rect.x;
    int current_y = client_rect.y;

    int i = 0;
    for(Container *c = _children.first(); c; c = c->next()) {
        Rect child_rect;

        int additional_space = 0;

        if (workspace()->maximized()) {
            if (hasFocus() && activeChild() == c)
                additional_space = available_space;
        } else
            additional_space = additional_space_per_child;

        if (isHorizontal()) {
            child_rect.x = current_x;
            child_rect.y = current_y;
            child_rect.w = c->minimumWidth() + additional_space + (2 * _child_frame_width);;
            child_rect.h = client_rect.h;

            current_x += child_rect.w;
        } else {
            child_rect.x = current_x;
            child_rect.y = current_y;
            child_rect.w = client_rect.w;
            child_rect.h = c->minimumHeight() + additional_space + (2 * _child_frame_width);

            current_y += child_rect.h;
        }

#if 0
        if (workspace()->maximized()) {
            if (hasFocus() && activeChild() == c)
                c->setRect(client_rect);
            else {
                Rect r(0,0,100,100);
                c->setRect(r);
            }
            c->layout();
        } else {
#endif

            Rect new_rect = child_rect;
            new_rect.x += _child_frame_width;
            new_rect.y += _child_frame_width;
            new_rect.w -= (2 * _child_frame_width);
            new_rect.h -= (2 * _child_frame_width);

            c->setRect(new_rect);
            c->layout();
#if 0
        }
#endif

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

ClientContainer *ContainerContainer::splitChild(Container *child, bool prepend_new_silbling)
{
    if (hierarchyDepth() >= _max_hierarchy_depth)
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
