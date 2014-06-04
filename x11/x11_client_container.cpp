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
#include "mouse_handler.h"
#include "common.h"


using namespace X11Global;


X11ClientContainer::X11ClientContainer() :
    ClientContainer(),
    _active_child_index(INVALID_INDEX),
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
    _active_child_index = INVALID_INDEX;

    for (int i = 0; i < numElements(); i++)
        _children[i]->setContainer(0);
    _children.clear();
}


int X11ClientContainer::indexOfChild(const Client *child)
{
    for(int i = 0; i < numElements(); i++) {
        if (child == _children[i])
            return i;
    }
    assert(false);
    abort();
}

int X11ClientContainer::addChild(Client *client)
{
    return addClient(static_cast<X11Client*>(client));
}

void X11ClientContainer::removeChild(Client *client)
{
    removeClient(static_cast<X11Client*>(client));
}

void X11ClientContainer::setActiveChild(int index)
{
    printvar(index);
//     if (client)
//         std::cout<<"ClientContainer::setActiveClient(): \""<<client->name()<<"\"\n";
//     else
//         std::cout<<"ClientContainer::setActiveClient(): 0\n";

    assert(index < numElements());
    assert(0 > index || _children[index]->isMapped());

    _active_child_index = index;

    if (activeClient())
        activeClient()->raise();

    parent()->handleSizeHintsChanged(this);
}

int X11ClientContainer::addClient(X11Client *client)
{
    assert(!client->container());

    client->setContainer(this);

    // make sure the active client stays on top of the stacking order
    if (activeClient())
        activeClient()->raise();

    _children.push_back(client);

    printvar(numElements());

    getLayout()->layoutContents();

    return numElements() - 1;
}

void X11ClientContainer::removeClient(X11Client *client)
{
    int index = indexOfChild(client);
    assert(index >= 0);

    client->setContainer(0);

    _children.erase(_children.begin() + index);

    if (_active_child_index >= numElements())
        _active_child_index = numElements() -1;

    if (activeClient())
        activeClient()->raise();

    getLayout()->layoutContents();
}

void X11ClientContainer::removeChildren(std::vector<Client*> &clients)
{
    for(size_t i = 0; i < _children.size(); i++)
        clients.push_back(_children[i]);
    clear();
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
    if (_is_mapped) //FIXME use isVisible()
        Theme::drawClientContainer(this, currentWidget()->canvas());
}


void X11ClientContainer::reparent(X11ContainerContainer *p)
{
    assert(!_workspace);
    assert(! (_parent && p) );

    _parent = p;

    X11ServerWidget *parent_widget = 0;
    if (p)
        parent_widget = p->widget();
    _widget->reparent(parent_widget);
    _minimized_widget->reparent(parent_widget);
}

void X11ClientContainer::applyMapState()
{
    printvar(_is_mapped);
    printvar(_workspace);

    if (!_is_mapped || !workspace()) {
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
    MouseHandler::handleClientContainerClick(this, ev.x, ev.y);
}

void X11ClientContainer::handleClientMap(X11Client *client)
{
    if (!activeClient()) {
        int index = indexOfChild(client);
        assert(index != INVALID_INDEX);
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
