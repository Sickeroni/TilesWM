#include "x11_client_container.h"

#include "x11_container_container.h"
#include "x11_client.h"
#include "x11_server_widget.h"
#include "x11_canvas.h"
#include "x11_global.h"
#include "x11_application.h"
#include "container_layout.h"
#include "workspace.h"
#include "theme.h"
#include "colors.h"
#include "common.h"


using namespace X11Global;


X11ClientContainer::X11ClientContainer(X11ContainerContainer *parent) : ClientContainer(parent),
    _active_child_index(-1),
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

void X11ClientContainer::clear()
{
    _active_child_index = -1;

    for (int i = 0; i < _children.size(); i++)
        delete _children[i];
    _children.clear();
}

int X11ClientContainer::indexOfChild(const Client *child)
{
    for(int i = 0; i < _children.size(); i++) {
        if (child == _children[i])
            return i;
    }
    return -1;
}

int X11ClientContainer::addClient(X11Client *client)
{
    debug;
    assert(!client->container());

    client->setContainer(this);

    // make sure the active client stays on top of the stacking order
    if (activeClient())
        activeClient()->raise();

    _children.push_back(client);

    printvar(_children.size());

    return _children.size() - 1;
}

void X11ClientContainer::setRect(const Rect &rect)
{
    currentWidget()->setRect(rect);
    ClientContainer::setRect(rect);
}

void X11ClientContainer::redraw()
{
    Theme::drawClientContainer(this, currentWidget()->canvas());
}

void X11ClientContainer::reparent(ContainerContainer *p)
{
    ClientContainer::reparent(p);
    _widget->reparent(static_cast<X11ContainerContainer*>(p)->widget());
    _minimized_widget->reparent(static_cast<X11ContainerContainer*>(p)->widget());
}

#if 0
void X11ClientContainer::setFocus()
{
    if (activeClient() && activeClient()->isMapped())
        activeClient()->setFocus();
    else // set focus to root
        XSetInputFocus(dpy(), X11Application::root(),
                       RevertToNone, CurrentTime);
    redraw();
}
#endif

void X11ClientContainer::handleButtonPress(const XButtonEvent &ev)
{
    debug;
//FIXME
#if 0
    if (activeClient())
        makeActive();
    handleMouseClick(ev.x_root, ev.y_root);
#endif
}

void X11ClientContainer::handleClientMap(X11Client *client)
{
    if (!activeClient()) {
        int index = indexOfChild(client);
        assert(-1 < index);
        setActiveChild(index);
    }
    getLayout()->layoutContents();
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
