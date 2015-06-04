#include "window_manager_3panel.h"
#include "client_container.h"
#include "container_layout.h"
#include "canvas.h"
#include "workspace.h"
#include "client_wrapper.h"

#include <cmath>

using std::max;
using std::min;

using namespace ContainerUtil;


class WindowManager3Panel::RootWidget : public ChildWidget
{
public:
    RootWidget() : ChildWidget(OTHER) {}
    virtual ~RootWidget() {}

    virtual void draw(Canvas *canvas) override {
        Rect rect = this->rect();
        rect.setPos(0, 0);
        canvas->begin();
        canvas->erase(rect);
        canvas->end();
    }
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
    _root(new RootWidget())
{
    workspace->addChild(_root);

    _master = new ClientContainer();
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
    _splitter1_pos = max(int(MIN_CONTAINER_SIZE), _splitter1_pos);
    _splitter2_pos = min(_splitter2_pos, h - (MIN_CONTAINER_SIZE + SPLITTER_WIDTH));
    _splitter2_pos = max(int(MIN_CONTAINER_SIZE), _splitter2_pos);

    printvar(_splitter1_pos);
    printvar(_splitter2_pos );
}

void WindowManager3Panel::setActive(bool active)
{
    WindowManager::setActive(active);
    _root->setMapped(active);
    layout();
}

void WindowManager3Panel::setHasFocus(bool has_focus)
{
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

//     ClientContainer *container = activeClientContainer();
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
                _slave1 = new ClientContainer();
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
                _slave2 = new ClientContainer();
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
    _splitter1_pos = max(int(MIN_CONTAINER_SIZE), _splitter1_pos);

    layout();
}

void WindowManager3Panel::moveSplitter2(int delta)
{
    _splitter2_pos += delta;

    _splitter2_pos = min(_splitter2_pos, workspace()->rect().h - (MIN_CONTAINER_SIZE + SPLITTER_WIDTH));
    _splitter2_pos = max(int(MIN_CONTAINER_SIZE), _splitter2_pos);

    layout();
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
