#ifndef __WINDOW_MANAGER_3PANEL_H__
#define __WINDOW_MANAGER_3PANEL_H__

#include "window_manager.h"
#include "container_util.h"

#include <unordered_map>

class ClientContainer;
class ChildWidget;

class WindowManager3Panel final : public WindowManager
{
public:
    WindowManager3Panel(Workspace *workspace, Mode *mode);
    ~WindowManager3Panel();

    virtual void layout() override;
    virtual ClientWrapper *activeClient() override;
    virtual void redrawAll() override;
    virtual void setActive(bool active) override;
    virtual void setHasFocus(bool has_focus) override;
    virtual void handleWorkspaceSizeChanged() override;

    static void createActions(ActionSet &actions);

protected:
    virtual void performAction(int id) override;
    virtual void performComplexAction(const ComplexAction */*action*/, const ComplexAction::Parameters */*parameters*/) override {}
    virtual void manageClient(ClientWrapper *client) override;
    virtual void unmanageClient(ClientWrapper *client) override;
    virtual void makeClientActive(ClientWrapper *client) override;

private:
    class RootWidget;

    enum {
        ACTION_MOVE_CLIENT_LEFT,
        ACTION_MOVE_CLIENT_RIGHT,
        ACTION_MOVE_CLIENT_UP,
        ACTION_MOVE_CLIENT_DOWN,
        ACTION_MOVE_HSPLIT_LEFT,
        ACTION_MOVE_HSPLIT_RIGHT,
        ACTION_MOVE_HSPLIT_UP,
        ACTION_MOVE_HSPLIT_DOWN,
        ACTION_TOGGLE_PRIMARY_EXPANDING,
        ACTION_TOGGLE_SECONDARY_EXPANDING,
        ACTION_SET_PRIMARY_SLAVE_TO_MINIMUM,
        ACTION_SET_SECONDARY_SLAVE_TO_MINIMUM
    };

    enum {
        SPLITTER_WIDTH = 8,
        MIN_CONTAINER_SIZE = 100
    };

    ClientContainer *activeClientContainer() {
        return _active_container;
    }

    void moveClient(ContainerUtil::Direction direction);

    void moveSplitter1(int delta);
    void moveSplitter2(int delta);

//     void togglePrimaryExpanding();
//     void toggleSecondaryExpanding();
//
//     void setPrimarySlaveToMinimum();
//     void setSecondarySlaveToMinimum();

    void makeContainerActive(ClientContainer *container);

    RootWidget *_root = 0;
    ClientContainer *_master = 0, *_slave1 = 0, *_slave2 = 0, *_active_container = 0;
    std::unordered_map<ClientWrapper*, ClientContainer*> _container_of_client;
    int _splitter1_pos = 0, _splitter2_pos = 0;
};

#endif
