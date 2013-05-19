#include "x11_client_container.h"

#include "x11_container_container.h"
#include "x11_client.h"
#include "x11_server_widget.h"
#include "x11_canvas.h"
#include "x11_global.h"
#include "x11_application.h"
#include "workspace.h"
#include "colors.h"

#include <iostream>


using namespace X11Global;


X11ClientContainer::X11ClientContainer(X11ContainerContainer *parent) :
    ClientContainer(parent),
    _widget(X11ServerWidget::create(parent->widget(),
                                    Colors::CLIENT_CONTAINER,
                                    this, ButtonPressMask | ExposureMask)),
    _minimized_widget(X11ServerWidget::create(parent->widget(),
                                              Colors::CLIENT_CONTAINER,
                                              this, ButtonPressMask | ExposureMask))
{
    _widget->map();
}

X11ClientContainer::~X11ClientContainer()
{
    clear();
    delete _minimized_widget;
    _minimized_widget = 0;
    delete _widget;
    _widget = 0;
}

void X11ClientContainer::setRect(const Rect &rect)
{
    currentWidget()->setRect(rect);
    ClientContainer::setRect(rect);
}

void X11ClientContainer::redraw()
{
    draw(currentWidget()->canvas());
}

void X11ClientContainer::reparent(ContainerContainer *p)
{
    ClientContainer::reparent(p);
    _widget->reparent(static_cast<X11ContainerContainer*>(p)->widget());
    _minimized_widget->reparent(static_cast<X11ContainerContainer*>(p)->widget());
}

void X11ClientContainer::setFocus()
{
    if (activeClient() && activeClient()->isMapped())
        activeClient()->setFocus();
    else // set focus to root
        XSetInputFocus(dpy(), X11Application::root(),
                       RevertToNone, CurrentTime);
    redraw();
}

void X11ClientContainer::handleButtonPress(const XButtonEvent &ev)
{
    std::cout<<"X11ClientContainer::handleButtonPress()\n";
    if (activeClient())
        makeActive();
    handleMouseClick(ev.x_root, ev.y_root);
}

void X11ClientContainer::handleActiveChanged()
{
    if (isMinimized()) {
        _widget->unmap();
        _minimized_widget->map();
    } else {
        _minimized_widget->unmap();
        _widget->map();
    }
}

void X11ClientContainer::handleMaximizedChanged()
{
    handleActiveChanged();
}

int X11ClientContainer::maxTextHeight()
{
    return currentWidget()->canvas()->maxTextHeight();
}
