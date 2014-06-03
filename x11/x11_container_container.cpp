#include "x11_container_container.h"

#include "x11_client_container.h"
#include "x11_server_widget.h"
#include "x11_canvas.h"
#include "x11_application.h"
#include "x11_global.h"
#include "container_layout.h"
#include "theme.h"
#include "colors.h"
#include "workspace.h"

#include <stdlib.h>


using namespace X11Global;


X11ContainerContainer::X11ContainerContainer() :
    ContainerContainer(),
    _active_child_index(INVALID_INDEX),
    _widget(X11ServerWidget::create(0,
                                    Colors::CONTAINER,
                                    this, ExposureMask))
{
}

X11ContainerContainer::~X11ContainerContainer()
{
    clear();
    delete _widget;
    _widget = 0;
}

int X11ContainerContainer::indexOfChild(const Container *child)
{
    for (int i = 0; i < numElements(); i++) {
        if (child == _children[i])
            return i;
    }
    assert(false);
    abort();
}

void X11ContainerContainer::clear()
{
    _active_child_index = INVALID_INDEX;

    for (int i = 0; i < numElements(); i++)
        delete _children[i];
    _children.clear();
}

void X11ContainerContainer::reparentContainer(Container *container, X11ContainerContainer *parent)
{
    switch(container->type())
    {
        case CONTAINER:
            static_cast<X11ContainerContainer*>(container)->reparent(parent);
            break;
        case CLIENT:
            static_cast<X11ClientContainer*>(container)->reparent(parent);
            break;
        default:
            assert(0);
            abort();
    }
}

int X11ContainerContainer::addChild(Container *container)
{
    assert(!container->parent());

    reparentContainer(container, this);

    _children.push_back(container);
    printvar(numElements());

    container->handleMaximizedChanged();
    container->setMapped(true);

    getLayout()->layoutContents();

    return numElements() - 1;
}

void X11ContainerContainer::insertChild(Container *container, int insert_pos)
{
    assert(!container->parent());

    reparentContainer(container, this);

    assert(insert_pos <= numElements());
    _children.insert(_children.begin() + insert_pos, container);

    container->handleMaximizedChanged();
    container->setMapped(true);

    getLayout()->layoutContents();
}

Container *X11ContainerContainer::replaceChild(int index, Container *new_child)
{
    assert(!new_child->parent());
    assert(index < numElements());

    Container *old_child = _children[index];

    old_child->setMapped(false);
    old_child->handleMaximizedChanged();

    reparentContainer(old_child, 0);
    reparentContainer(new_child, this);

    _children[index] = new_child;

    new_child->handleMaximizedChanged();
    new_child->setMapped(true);

    getLayout()->layoutContents();

    return old_child;
}

Container *X11ContainerContainer::removeChild(int index)
{
    assert(index < numElements());

    Container *child = _children[index];

    child->handleMaximizedChanged();
    child->setMapped(false);

    reparentContainer(child, 0);

    _children.erase(_children.begin() + index);

    if (_active_child_index >= numElements())
        _active_child_index = numElements() -1;

    return child;
}

void X11ContainerContainer::setActiveChild(int index)
{
    assert(index < numElements());

    if (index == _active_child_index)
        return;

    if (index < numElements()) {
        Container *old_active = 0;
        if (_active_child_index != INVALID_INDEX)
            old_active = _children[_active_child_index];

        _active_child_index = index;

        if (old_active)
            old_active->handleActiveChanged();

        if (_active_child_index != INVALID_INDEX)
            _children[_active_child_index]->handleActiveChanged();

        if (workspace() && workspace()->maximized() && isActive())
            getLayout()->layoutContents();
    }

    redraw();
}

void X11ContainerContainer::setMapped(bool mapped)
{
    if (mapped)
        _widget->map();
    else
        _widget->unmap();
}

void X11ContainerContainer::setRect(const Rect &rect)
{
    _widget->setRect(rect);
    ContainerContainer::setRect(rect);
}

void X11ContainerContainer::redraw()
{
    if (_widget->isMapped()) //FIXME use isVisible()
        Theme::drawContainerContainer(this, _widget->canvas());
}

void X11ContainerContainer::reparent(X11ContainerContainer *p)
{
    assert(!_workspace);
    assert(! (_parent && p) );

    _parent = p;

    X11ServerWidget *parent_widget = 0;
    if (p)
        parent_widget = p->widget();
    _widget->reparent(parent_widget);
}

int X11ContainerContainer::maxTextHeight() const
{
    return _widget->canvas()->maxTextHeight();
}
