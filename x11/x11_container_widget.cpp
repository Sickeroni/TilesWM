#include "x11_container_widget.h"

#include "theme.h"
#include "canvas.h"
#include "colors.h"
#include "mouse_handler.h"

X11ContainerWidget::X11ContainerWidget(ContainerBase *container) : ContainerWidget(container),
    _widget(X11ServerWidget::create(
        0,
        Colors::CLIENT_CONTAINER,
        this, ButtonPressMask | ExposureMask)),
    _minimized_widget(X11ServerWidget::create(
        0,
        Colors::CLIENT_CONTAINER,
        this, ButtonPressMask | ExposureMask))
{
}

X11ContainerWidget::~X11ContainerWidget()
{
    delete _minimized_widget;
    _minimized_widget = 0;
    delete _widget;
    _widget = 0;
}

void X11ContainerWidget::redraw()
{
    if (_is_mapped) //FIXME use isVisible()
        Theme::drawContainer(_container, currentWidget()->canvas());
}
#if 0
void X11ContainerWidget::addChild(WidgetBase *child)
{
    abort();
}

void X11ContainerWidget::removeChild(WidgetBase *child)
{
    abort();
}

void X11ContainerWidget::addClient(Client *client)
{
    abort();
//     assert(!client->container());

//     client->setContainerWidget(this);

    // make sure the active client stays on top of the stacking order
//     if (activeClient())
//         static_cast<X11Client*>(activeClient())->raise();

//     _children.push_back(client);

//     printvar(numElements());

//     getLayout()->layoutContents();

//     return numElements() - 1;
}

void X11ContainerWidget::removeClient(Client *client)
{
    abort();
#if 0
    int index = indexOfChild(client);
    assert(index >= 0);

    client->setContainer(0);

    _children.erase(_children.begin() + index);

    if (_active_child_index >= numElements())
        _active_child_index = numElements() -1;

    if (activeClient())
        static_cast<X11Client*>(activeClient())->raise();

    getLayout()->layoutContents();
#endif
}
#endif

void X11ContainerWidget::setMapped(bool mapped)
{
    _is_mapped = mapped;
    applyMapState();
}

void X11ContainerWidget::setMinimized(bool minimized)
{
    _is_minimized = minimized;
    applyMapState();
}

void X11ContainerWidget::setRect(const Rect &rect)
{
    currentWidget()->setRect(rect);
}

void X11ContainerWidget::reparent(WidgetBase *parent)
{
    X11ContainerWidget *new_parent = 0;

    if (parent) {
        if (parent->type() == WidgetBase::CONTAINER)
            new_parent = static_cast<X11ContainerWidget*>(parent);
        else
            abort();
    }

    reparent(new_parent);
}

void X11ContainerWidget::reparent(X11ContainerWidget *p)
{
//     assert(! (_parent && p) );

//     _parent = p;

    X11ServerWidget *parent_widget = 0;
    if (p)
        parent_widget = p->x11Widget();
    _widget->reparent(parent_widget);
    _minimized_widget->reparent(parent_widget);
}

void X11ContainerWidget::handleButtonPress(const XButtonEvent &ev)
{
    MouseHandler::handleContainerClick(_container, ev.x, ev.y);
}

int X11ContainerWidget::maxTextHeight() const
{
    return const_cast<X11ContainerWidget*>(this)->currentWidget()->canvas()->maxTextHeight();
}

void X11ContainerWidget::applyMapState()
{
    printvar(_is_mapped);

    if (!_is_mapped) {
        _widget->unmap();
        _minimized_widget->unmap();
    } else if (isMinimized()) {
        _widget->unmap();
        _minimized_widget->map();
    } else {
        _minimized_widget->unmap();
        _widget->map();
    }
}
