#include "window_manager_3panel.h"
#include "client_container.h"
#include "container_layout.h"
#include "canvas.h"
#include "workspace.h"
#include "widget_util.h"
#include "client_wrapper.h"

#include <cmath>

using namespace IntegerUtil;
using namespace ContainerUtil;

class WindowManager3Panel::RootWidget : public ChildWidget
{
public:
    RootWidget(WindowManager3Panel *wm) : ChildWidget(OTHER), _wm(wm) {}
    virtual ~RootWidget() {}

    virtual void draw(Canvas *canvas) override {
        Rect rect = this->rect();
        rect.setPos(0, 0);
        canvas->begin();
        canvas->erase(rect);
        canvas->end();
    }
    virtual void handleButtonPress(int /*x_global*/, int /*y_global*/, int /*button*/) override {}
    virtual void handleButtonRelease(int /*button*/) override  {
        _wm->finishDrag();
    }
    virtual void handleMouseMove(int x_global, int y_global) override  {
        _wm->handleMouseMove(x_global, y_global);
    }

private:
    WindowManager3Panel *_wm;
};


void WindowManager3Panel::createActions(ActionSet &actions)
{
    actions.createAction("moveClientLeft", ACTION_MOVE_CLIENT_LEFT);
    actions.createAction("moveClientRight", ACTION_MOVE_CLIENT_RIGHT);
    actions.createAction("moveClientUp", ACTION_MOVE_CLIENT_UP);
    actions.createAction("moveClientDown", ACTION_MOVE_CLIENT_DOWN);

    actions.createAction("moveHSplitLeft", ACTION_MOVE_HSPLIT_LEFT);
    actions.createAction("moveHSplitRight", ACTION_MOVE_HSPLIT_RIGHT);
    actions.createAction("moveVSplitUp", ACTION_MOVE_HSPLIT_UP);
    actions.createAction("moveVSplitDown", ACTION_MOVE_HSPLIT_DOWN);

    actions.createAction("togglePrimaryExpanding", ACTION_TOGGLE_PRIMARY_EXPANDING);
    actions.createAction("toggleSecondaryExpanding", ACTION_TOGGLE_SECONDARY_EXPANDING);

    actions.createAction("setPrimarySlaveToMinimum", ACTION_SET_PRIMARY_SLAVE_TO_MINIMUM);
    actions.createAction("setSecondarySlaveToMinimum", ACTION_SET_SECONDARY_SLAVE_TO_MINIMUM);
}

WindowManager3Panel::WindowManager3Panel(Workspace *workspace, Mode *mode) :
    WindowManager(workspace, mode),
    _root(new RootWidget(this))
{
    workspace->addChild(_root);

    _master = createClientContainer();
    _master->reparent(_root);
    _master->setMapped(true);

    _active_container = _master;
}

WindowManager3Panel::~WindowManager3Panel()
{
    _active_container = 0;
    delete _slave2;
    _slave2 = 0;
    delete _slave1;
    _slave1 = 0;
    delete _master;
    _master = 0;
    workspace()->removeChild(_root);
    delete _root;
    _root= 0;
}

void WindowManager3Panel::handleWorkspaceSizeChanged()
{
    int w = workspace()->rect().w;
    int h = workspace()->rect().h;

    printvar(w);
    printvar(h);

    _splitter1_pos = max(0, w - SPLITTER_WIDTH) / 2;
    _splitter2_pos = max(0, h - SPLITTER_WIDTH) / 2;

    _splitter1_pos = min(_splitter1_pos, w - (MIN_CONTAINER_SIZE + SPLITTER_WIDTH));
    _splitter1_pos = max(MIN_CONTAINER_SIZE, _splitter1_pos);
    _splitter2_pos = min(_splitter2_pos, h - (MIN_CONTAINER_SIZE + SPLITTER_WIDTH));
    _splitter2_pos = max(MIN_CONTAINER_SIZE, _splitter2_pos);

    printvar(_splitter1_pos);
    printvar(_splitter2_pos );
}

void WindowManager3Panel::setActive(bool active)
{
    if (!active)
        cancelDrag();
    WindowManager::setActive(active);
    _root->setMapped(active);
    layout();
}

void WindowManager3Panel::setHasFocus(bool has_focus)
{
    if (!has_focus)
        cancelDrag();
    if (_active_container)
        _active_container->setHasFocus(has_focus);
}

void WindowManager3Panel::layout()
{
    assert(_splitter1_pos > 0);
    assert(_splitter2_pos > 0);

    int w = workspace()->rect().w;
    int h = workspace()->rect().h;

    assert(_splitter1_pos < w);
    assert(_splitter2_pos < h);

    if (w > 0 && h > 0) {
        _root->setRect(Rect(0, 0, w, h));

        int slave_x = _splitter1_pos + SPLITTER_WIDTH;
        int slave_w = w - slave_x;
        int slave2_y = _splitter2_pos + SPLITTER_WIDTH;
        int slave2_h = h - slave2_y;

        if (_slave1 || _slave2) {
            _master->setRect(Rect(0, 0, _splitter1_pos, h));
            if (_slave1 && _slave2) {
                _slave1->setRect(Rect(slave_x, 0, slave_w, _splitter2_pos));
                _slave2->setRect(Rect(slave_x, slave2_y, slave_w, slave2_h));
            } else {
                assert(_slave1);
                _slave1->setRect(Rect(slave_x, 0, slave_w, h));
            }
        } else {
            _master->setRect(Rect(0, 0, w, h));
        }

        _master->getLayout()->layoutContents();
        if (_slave1)
            _slave1->getLayout()->layoutContents();
        if (_slave2)
            _slave2->getLayout()->layoutContents();
    }
}

void WindowManager3Panel::manageClient(ClientWrapper *client)
{
    debug;

    assert(_container_of_client[client] == 0);

    ClientContainer *container = _master;
    assert(container);

    container->addChild(client);
    _container_of_client[client] = container;

}

void WindowManager3Panel::unmanageClient(ClientWrapper *client)
{
    assert(client->workspace() == workspace());

    auto it = _container_of_client.find(client);
    assert(it != _container_of_client.end());

    ClientContainer *container = it->second;

    container->removeChild(client);

    _container_of_client.erase(it);
}

ClientWrapper *WindowManager3Panel::activeClient()
{
    return activeClientContainer()->activeClient();
}

void WindowManager3Panel::makeClientActive(ClientWrapper *client)
{
    printvar(client);
    ClientContainer *container = _container_of_client[client];
    assert(container);

    makeContainerActive(container);
    container->setActiveChild(container->indexOfChild(client));
}

void WindowManager3Panel::redrawAll()
{
    _root->redraw();
    _master->redrawAll();
    if (_slave1)
        _slave1->redrawAll();
    if (_slave2)
        _slave2->redrawAll();
}

void WindowManager3Panel::performAction(int id)
{
    printvar(id);
    switch(id) {
        case ACTION_MOVE_CLIENT_LEFT:
            moveClient(LEFT);
            break;
        case ACTION_MOVE_CLIENT_RIGHT:
            moveClient(RIGHT);
            break;
        case ACTION_MOVE_CLIENT_UP:
            moveClient(UP);
            break;
        case ACTION_MOVE_CLIENT_DOWN:
            moveClient(DOWN);
            break;
        case ACTION_MOVE_HSPLIT_RIGHT:
            moveSplitter1(100);
            break;
        case ACTION_MOVE_HSPLIT_LEFT:
            moveSplitter1(-100);
            break;
        case ACTION_MOVE_HSPLIT_UP:
            moveSplitter2(-100);
            break;
        case ACTION_MOVE_HSPLIT_DOWN:
            moveSplitter2(100);
            break;
        case ACTION_TOGGLE_PRIMARY_EXPANDING:
//             togglePrimaryExpanding();
            break;
        case ACTION_TOGGLE_SECONDARY_EXPANDING:
//             toggleSecondaryExpanding();

            break;
        case ACTION_SET_PRIMARY_SLAVE_TO_MINIMUM:
//             setPrimarySlaveToMinimum();
            break;
        case ACTION_SET_SECONDARY_SLAVE_TO_MINIMUM:
//             setSecondarySlaveToMinimum();
            break;
    }
}

void WindowManager3Panel::makeContainerActive(ClientContainer *container)
{
    assert(container);
    _active_container = container;
}

void WindowManager3Panel::moveClient(Direction direction)
{
    ClientContainer *container = activeClientContainer();

    ClientWrapper *client = container->activeClient();
    printvar(client);
    if (!client)
        return;

    assert(_container_of_client[client] == container);

    ClientContainer *target = 0;

    if (container == _master) {
        if (direction == RIGHT) {
            if (!_slave1) {
                _slave1 = createClientContainer();
                _slave1->reparent(_root);
                _slave1->setMapped(true);
            }
            target = _slave1;
        }
    } else {
        if (direction == LEFT) {
            target = _master;
        } else if (container == _slave1 && direction == DOWN) {
            if (!_slave2) {
                _slave2 = createClientContainer();
                _slave2->reparent(_root);
                _slave2->setMapped(true);
            }
            target = _slave2;
        } else if (container == _slave2 && direction == UP) {
            assert(_slave1);
            target = _slave1;
        }
    }

    printvar(target);

    if (target) {
        container->removeChild(client);
        Container::Index index = target->addChild(client);
        target->setActiveChild(index);
        _container_of_client[client] = target;

        makeContainerActive(target);

        if (_slave2 && _slave2->isEmpty()) {
            delete _slave2;
            _slave2 = 0;
        }
        if (!_slave2 && _slave1 && _slave1->isEmpty()) {
            delete _slave1;
            _slave1 = 0;
        }

        layout();
        client->setFocus();
    }
}

void WindowManager3Panel::moveSplitter1(int delta)
{
    _splitter1_pos += delta;

    _splitter1_pos = min(_splitter1_pos, workspace()->rect().w - (MIN_CONTAINER_SIZE + SPLITTER_WIDTH));
    _splitter1_pos = max(MIN_CONTAINER_SIZE, _splitter1_pos);

    layout();
}

void WindowManager3Panel::moveSplitter2(int delta)
{
    _splitter2_pos += delta;

    _splitter2_pos = min(_splitter2_pos, workspace()->rect().h - (MIN_CONTAINER_SIZE + SPLITTER_WIDTH));
    _splitter2_pos = max(MIN_CONTAINER_SIZE, _splitter2_pos);

    layout();
}

ClientContainer *WindowManager3Panel::createClientContainer()
{
    ClientContainer *c = new ClientContainer();
    c->setClientDragHandler(this);
    return c;
}

void WindowManager3Panel::handleClientDragStart(ClientWrapper *client, int x_global, int y_global, Client::DragMode mode)
{
    printvar(x_global);
    printvar(y_global);
    printvar(mode);

    //FIXME what if the pointer is already grabbed ?
    assert(_dragged_splitter == SPLITTER_NONE);

    if (mode != Client::DRAG_RESIZE)
        return;

    int x_local = x_global;
    int y_local = y_global;
    _root->globalToLocal(x_local, y_local);

    int x_client = x_global;
    int y_client = y_global;
    client->globalToLocal(x_client, y_client);

#if 0
    bool drag_vsplit = (_slave1 || _slave2) && x_local >= (_splitter1_pos - 200) && x_local  < (_splitter1_pos + 200);
    bool drag_hsplit = _slave2 && x_local > _splitter1_pos && y_local >= (_splitter2_pos - 200) && y_local < (_splitter2_pos + 200);
#else
    bool drag_vsplit = false;
    bool drag_hsplit = false;

    if (_slave1 || _slave2) {
        int master_drag_area = max(MIN_DRAG_AREA, _splitter1_pos / 2);
        int slave_drag_area = max(MIN_DRAG_AREA, (workspace()->rect().w - _splitter1_pos) / 2);
        if (x_local < _splitter1_pos && x_local > (_splitter1_pos - master_drag_area))
            drag_vsplit = true;
        else if(x_local >= _splitter1_pos && x_local < (_splitter1_pos + slave_drag_area))
            drag_vsplit = true;
    }
    if (_slave2 && x_local > _splitter1_pos) {
        int slave1_drag_area = max(MIN_DRAG_AREA, _splitter2_pos / 2);
        int slave2_drag_area = max(MIN_DRAG_AREA, (workspace()->rect().h - _splitter2_pos) / 2);
        if (y_local < _splitter2_pos && y_local > (_splitter2_pos - slave1_drag_area))
            drag_hsplit = true;
        else if(y_local >= _splitter2_pos && y_local < (_splitter2_pos + slave2_drag_area))
            drag_hsplit = true;
    }
#endif

    if (drag_hsplit && drag_vsplit)
        _dragged_splitter = SPLITTER_BOTH;
    else if (drag_hsplit)
        _dragged_splitter = SPLITTER_HORIZONTAL;
    else if (drag_vsplit)
        _dragged_splitter = SPLITTER_VERTICAL;
    else
        _dragged_splitter = SPLITTER_NONE;

    unsigned int dragged_edge = WidgetUtil::EDGE_NONE;
    if (drag_vsplit) {
        if (x_local < _splitter1_pos)
            dragged_edge |= WidgetUtil::EDGE_RIGHT;
        else
            dragged_edge |= WidgetUtil::EDGE_LEFT;
    }
    if(drag_hsplit) {
        if (y_local < _splitter2_pos)
            dragged_edge |= WidgetUtil::EDGE_BOTTOM;
        else
            dragged_edge |= WidgetUtil::EDGE_TOP;
    }

    if (_dragged_splitter != SPLITTER_NONE) {
        _drag_start_x = x_local;
        _drag_start_y = y_local;
        _root->grabMouse(WidgetUtil::resizeCursorForEdge(dragged_edge));
    }
}

void WindowManager3Panel::finishDrag()
{
    if (_dragged_splitter != SPLITTER_NONE) {
        _drag_start_x = 0;
        _drag_start_y = 0;
        _dragged_splitter = SPLITTER_NONE;
        _root->releaseMouse();
    }
}

void WindowManager3Panel::handleMouseMove(int x_global, int y_global)
{
    if (_dragged_splitter != SPLITTER_NONE) {
        int x = x_global;
        int y = y_global;
        _root->globalToLocal(x, y);

        if (_dragged_splitter & SPLITTER_VERTICAL)
            _splitter1_pos += (x - _drag_start_x);
        if (_dragged_splitter & SPLITTER_HORIZONTAL)
            _splitter2_pos += (y - _drag_start_y);

        _splitter1_pos = min(_splitter1_pos, workspace()->rect().w - (MIN_CONTAINER_SIZE + SPLITTER_WIDTH));
        _splitter1_pos = max(MIN_CONTAINER_SIZE, _splitter1_pos);
        _splitter2_pos = min(_splitter2_pos, workspace()->rect().h - (MIN_CONTAINER_SIZE + SPLITTER_WIDTH));
        _splitter2_pos = max(MIN_CONTAINER_SIZE, _splitter2_pos);

        layout();

        _drag_start_x = x;
        _drag_start_y = y;
    }
}

#if 0
void WindowManager3Panel::moveSplitter(bool horizontal, int delta)
{
    ContainerContainer *root = Application::activeWorkspace()->rootContainer();

    if (horizontal && root->isHorizontal()) {
        Container *c =  root->child(1);
        c->setFixedWidth(c->fixedWidth() - delta);
    } else if (!horizontal && root->isHorizontal()) {
        Container *c = root->child(1)->toContainerContainer()->child(1);
        c->setFixedHeight(c->fixedHeight() - delta);
    }
}

void WindowManager3Panel::togglePrimaryExpanding()
{
    ContainerContainer *root = Application::activeWorkspace()->rootContainer();


    Container *c = root->child(1);
    c->enableFixedSize(!c->isFixedSize());
}

void WindowManager3Panel::toggleSecondaryExpanding()
{
    ContainerContainer *root = Application::activeWorkspace()->rootContainer();

    Container *c = root->child(1)->toContainerContainer()->child(1);
    c->enableFixedSize(!c->isFixedSize());
}

void WindowManager3Panel::setPrimarySlaveToMinimum()
{
    ContainerContainer *root = Application::activeWorkspace()->rootContainer();

    ContainerContainer *container = root->child(1)->toContainerContainer();
    if (Client *client = container->child(0)->toClientContainer()->activeClient()) {
        container->setFixedWidth(client->minWidth());
        container->setFixedHeight(client->minHeight());
        container->enableFixedSize(true);
    }
}

void WindowManager3Panel::setSecondarySlaveToMinimum()
{
    ContainerContainer *root = Application::activeWorkspace()->rootContainer();

    ClientContainer *container = root->child(1)->toContainerContainer()->child(1)->toClientContainer();
    if (Client *client = container->activeClient()) {
        container->setFixedWidth(client->minWidth());
        container->setFixedHeight(client->minHeight());
        container->enableFixedSize(true);
    }
}
#endif
