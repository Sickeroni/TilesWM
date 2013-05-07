#include "x11_client_container.h"

#include "x11_container_container.h"
#include "x11_client.h"
#include "x11_server_widget.h"
#include "x11_canvas.h"
#include "x11_global.h"
#include "x11_application.h"
#include "workspace.h"


using namespace X11Global;


X11ClientContainer::X11ClientContainer(X11ContainerContainer *parent) :
    ClientContainer(parent),
    _widget(X11ServerWidget::create(parent->widget(), this, ExposureMask))
{
    _widget->map();
}

X11ClientContainer::~X11ClientContainer()
{
    clear();
    delete _widget;
    _widget = 0;
}

void X11ClientContainer::setRect(const Rect &rect)
{
    _widget->setRect(rect);
    ClientContainer::setRect(rect);
}

void X11ClientContainer::redraw()
{
    draw(_widget->canvas());
}

void X11ClientContainer::reparent(ContainerContainer *p)
{
    ClientContainer::reparent(p);
    _widget->reparent(static_cast<X11ContainerContainer*>(p)->widget());
}

void X11ClientContainer::setFocus()
{
    if (activeClient())
        activeClient()->setFocus();
    else // set focus to root
        XSetInputFocus(dpy(), X11Application::root(),
                       RevertToNone, CurrentTime);
    redraw();
}

void X11ClientContainer::handleActiveChanged()
{
#if 0
    if (workspace()->maximized()) {
        if (parent()->activeChild() != this || !parent()->hasFocus())
            _widget->unmap();
        else if (parent()->activeChild() == this)
            _widget->map();
    } else
        _widget->map();
#endif

    //FIXME this dis hacky
    if (isMinimized()) {
        for (Client *c = _clients.first(); c; c = c->next())
            static_cast<X11Client*>(c)->unmap();
    } else {
        for (Client *c = _clients.first(); c; c = c->next())
            static_cast<X11Client*>(c)->map();
    }
}

void X11ClientContainer::handleMaximizedChanged()
{
    handleActiveChanged();
}

int X11ClientContainer::maxTextHeight()
{
    return _widget->canvas()->maxTextHeight();
}
