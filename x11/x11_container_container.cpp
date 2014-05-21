#include "x11_container_container.h"

#include "x11_client_container.h"
#include "x11_server_widget.h"
#include "x11_canvas.h"
#include "x11_application.h"
#include "x11_global.h"
#include "theme.h"
#include "colors.h"

#include <stdlib.h>


using namespace X11Global;


X11ContainerContainer *X11ContainerContainer::create(Workspace *workspace)
{
    return new X11ContainerContainer(workspace, 0);
}


X11ContainerContainer::X11ContainerContainer(Workspace *workspace, X11ContainerContainer *parent) :
    ContainerContainer(parent),
//     _active_child(0),
    _widget(X11ServerWidget::create(parent ? parent->widget() : 0,
                                    Colors::CONTAINER,
                                    this, ExposureMask))
{
    setWorkspace(workspace);
    _widget->map();
}

X11ContainerContainer::~X11ContainerContainer()
{
    clear();
    delete _widget;
    _widget = 0;
}


void X11ContainerContainer::clear()
{
//     _active_child = 0;

    for (int i = 0; i < _children.size(); i++)
        delete _children[i];
    _children.clear();
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


void X11ContainerContainer::setRect(const Rect &rect)
{
    _widget->setRect(rect);
    ContainerContainer::setRect(rect);
}

#if 0
ClientContainer *X11ContainerContainer::createClientContainer()
{
    X11ClientContainer *client_container = new X11ClientContainer(this);
    return client_container;
}

ContainerContainer *X11ContainerContainer::createContainerContainer()
{
    X11ContainerContainer *container = new X11ContainerContainer(workspace(), this);
    return container;
}
#endif

void X11ContainerContainer::redraw()
{
    Theme::drawContainerContainer(this, _widget->canvas());
}

void X11ContainerContainer::reparent(ContainerContainer *p)
{
    ContainerContainer::reparent(p);
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
