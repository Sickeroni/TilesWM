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
    _active_child_index(-1),
//     _active_child(0),
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
    for (int i = 0; i < _children.size(); i++) {
        if (child == _children[i])
            return i;
    }
    return -1;
}

void X11ContainerContainer::clear()
{
    _active_child_index = -1;

    for (int i = 0; i < _children.size(); i++)
        delete _children[i];
    _children.clear();
}

int X11ContainerContainer::addChild(Container *container)
{
    assert(!container->parent());

    switch(container->type())
    {
        case CONTAINER:
            static_cast<X11ContainerContainer*>(container)->reparent(this);
            break;
        case CLIENT:
            static_cast<X11ClientContainer*>(container)->reparent(this);
            break;
        default:
            assert(0);
            abort();
    }

    _children.push_back(container);
    printvar(_children.size());

    container->setMapped(true);

    getLayout()->layoutContents();

    return _children.size() - 1;
}

void X11ContainerContainer::setActiveChild(int index)
{
    assert(index < _children.size());

    if (_active_child_index == index)
        return;

    if (index < _children.size()) {
        if (-1 < _active_child_index)
            _children[_active_child_index]->handleActiveChanged();

        _active_child_index = index;

        if (-1 < _active_child_index)
            _children[_active_child_index]->handleActiveChanged();

        if (workspace()->maximized() && isActive())
            getLayout()->layoutContents();
    }
}

#if 0
void X11ContainerContainer::deleteChild(Container *child)
{
    if (child == _active_child)
        _active_child = 0;

    _children.remove(child);

    delete child;

    //FIXME should this be done here or by the caller ?
    if (!_active_child && _children.size())
        _active_child = _children[0]:
}
#endif

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
    Theme::drawContainerContainer(this, _widget->canvas());
}

void X11ContainerContainer::reparent(ContainerContainer *p)
{
    assert(!_workspace);
    _parent = p;
    _widget->reparent(static_cast<X11ContainerContainer*>(p)->widget());
}

void X11ContainerContainer::deleteEmptyChildren()
{
//     _widget->unmap();
//     ContainerContainer::deleteEmptyChildren(); //FIXME
//     _widget->map();
}

#if 0
void X11ContainerContainer::setFocus()
{
    if (activeChild())
        activeChild()->setFocus();
    else // set focus to root
        XSetInputFocus(dpy(), X11Application::root(),
                       RevertToNone, CurrentTime);
    redrawAll();
}
#endif
