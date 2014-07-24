#ifndef __WINDOW_MANAGER_DEFAULT_H__
#define __WINDOW_MANAGER_DEFAULT_H__

#include "window_manager.h"
#include "container_util.h"

#include <unordered_map>

class ContainerContainer;

class WindowManagerDefault final : public WindowManager
{
public:
    WindowManagerDefault(Workspace *workspace, Mode *mode);
    ~WindowManagerDefault();

    virtual void setActive(bool active) override;
    virtual void layout() override;
    virtual void manageClient(ClientWrapper *client) override;
    virtual void unmanageClient(ClientWrapper *client) override;
    virtual ClientWrapper *activeClient() override;
    virtual void makeClientActive(ClientWrapper *client) override;
    virtual void redrawAll() override;

    static void createActions(ActionSet &actions);

protected:
    enum {
        ACTION_MOVE_CLIENT_LEFT,
        ACTION_MOVE_CLIENT_RIGHT,
        ACTION_MOVE_CLIENT_UP,
        ACTION_MOVE_CLIENT_DOWN,
        ACTION_DELETE_EMPTY_CONTAINERS,
        ACTION_TOGGLE_EXPANDING,
        ACTION_TOGGLE_PARENT_EXPANDING,
        ACTION_INC_WIDTH,
        ACTION_DEC_WIDTH,
        ACTION_INC_HEIGHT,
        ACTION_DEC_HEIGHT,
        ACTION_SET_FIXED_SIZE_TO_MINIMUM,
        ACTION_ROTATE
    };

    virtual void performAction(int id) override;
    virtual void performComplexAction(const ComplexAction *action, const ComplexAction::Parameters *parameters) override {}

private:
    ClientContainer *activeClientContainer();
    void moveClient(ContainerUtil::Direction direction);
    void changeSize(bool horizontal, int delta);
    void setFixedSizeToMinimum();
    void setMaximizeActiveContainer(bool set);
    void makeContainerActive(Container *container);
    bool isContainerActive(Container *container);
    ClientContainer *containerOfClient(ClientWrapper *client);

    ContainerContainer *_root_container = 0;
    bool _maximize_active_container = false;
    std::unordered_map<ClientWrapper*, ClientContainer*> _container_of_client;
};

#endif
