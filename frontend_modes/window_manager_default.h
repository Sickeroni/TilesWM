#ifndef __WINDOW_MANAGER_DEFAULT_H__
#define __WINDOW_MANAGER_DEFAULT_H__

#include "window_manager.h"
#include "container_util.h"

class ContainerContainer;

class WindowManagerDefault final : public WindowManager
{
public:
    WindowManagerDefault(Workspace *workspace, std::string action_set_name);
    ~WindowManagerDefault();

    virtual void layout() override;
    virtual void manageClient(Client *client) override;
    virtual void unmanageClient(Client *client) override;
    virtual void unmanageAllClients(std::vector<Client*> &unmanaged_clients) override;
    virtual Client *activeClient() override;
    virtual void makeClientActive(Client *client) override;


private:
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
        ACTION_SET_FIXED_SIZE_TO_MINIMUM
    };

    ClientContainer *activeClientContainer();

    void moveClient(ContainerUtil::Direction direction);
    void changeSize(bool horizontal, int delta);
    void setFixedSizeToMinimum();
    void setMaximizeActiveContainer(bool set);
    void makeContainerActive(Container *container);
    bool isContainerActive(Container *container);

    ContainerContainer *_root_container = 0;
    bool _maximize_active_container = false;
};

#endif
