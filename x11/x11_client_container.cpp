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


X11ClientContainer::X11ClientContainer() :
    ClientContainer(),
    _active_child_index(-1),
    _widget(X11ServerWidget::create(0,
                                    Colors::CLIENT_CONTAINER,
                                    this, ButtonPressMask | ExposureMask)),
    _minimized_widget(X11ServerWidget::create(0,
                                              Colors::CLIENT_CONTAINER,
                                              this, ButtonPressMask | ExposureMask)),
    _is_mapped(false)
{
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
        _children[i]->setContainer(0);
    _children.clear();
}

#if 0
void X11ClientContainer::removeClientInt(Client *c, bool moving_to_new_container)
{
    debug;

    if (c == _active_client)
        unfocusActiveClient();

    _clients.remove(c);

    printvar(_clients.count());

    if (!moving_to_new_container)
        c->setContainer(0);

    if (c->isMapped())
        getLayout()->layoutContents();
    else
        redraw();

//FIXME
#if 0
    if (isEmpty() && _parent)
        _parent->setDirty(true);
#endif
}
#endif

int X11ClientContainer::indexOfChild(const Client *child)
{
    for(int i = 0; i < _children.size(); i++) {
        if (child == _children[i])
            return i;
    }
    return -1;
}

void X11ClientContainer::setActiveChild(int index)
{
//     if (client)
//         std::cout<<"ClientContainer::setActiveClient(): \""<<client->name()<<"\"\n";
//     else
//         std::cout<<"ClientContainer::setActiveClient(): 0\n";

    assert(index < _children.size());
    assert(0 > index || _children[index]->isMapped());

    _active_child_index = index;

    if (activeClient())
        activeClient()->raise();

    parent()->handleSizeHintsChanged(this);
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

    getLayout()->layoutContents();

    return _children.size() - 1;
}

void X11ClientContainer::removeClient(X11Client *client)
{
    int index = indexOfChild(client);
    assert(-1 < index);

    client->setContainer(0);

    _children.erase(_children.begin() + index);

    if (_children.size() && (-1 < _active_child_index)) {
        if (_active_child_index > (_children.size() - 1))
            _active_child_index = (_children.size() - 1);
    } else
        _active_child_index = -1;

    if (activeClient())
        activeClient()->raise();

    getLayout()->layoutContents();
}

void X11ClientContainer::setMapped(bool mapped)
{
    _is_mapped = mapped;
    applyMapState();
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


void X11ClientContainer::reparent(X11ContainerContainer *p)
{
    assert(!_workspace);

    _parent = p;

    X11ServerWidget *parent_widget = 0;
    if (p)
        parent_widget = p->widget();
    _widget->reparent(parent_widget);
    _minimized_widget->reparent(parent_widget);
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

void X11ClientContainer::applyMapState()
{
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

void X11ClientContainer::handleButtonPress(const XButtonEvent &ev)
{
    handleMouseClick(ev.x_root, ev.y_root);
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
    applyMapState();
}

void X11ClientContainer::handleMaximizedChanged()
{
    applyMapState();
}

int X11ClientContainer::maxTextHeight()
{
    return currentWidget()->canvas()->maxTextHeight();
}
