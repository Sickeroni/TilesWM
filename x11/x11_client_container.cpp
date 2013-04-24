#include "x11_client_container.h"

#include "x11_container_container.h"
#include "x11_server_widget.h"
#include "x11_canvas.h"


X11ClientContainer::X11ClientContainer(X11ContainerContainer *parent) :
    ClientContainer(parent),
    _widget(X11ServerWidget::create(parent->widget()))
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
    ClientContainer::setRect(rect);
    _widget->setRect(rect);
}

void X11ClientContainer::redraw()
{
    draw(_widget->canvas());
}