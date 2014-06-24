#ifndef __WINDOW_MANAGER_H__
#define __WINDOW_MANAGER_H__

#include "action_set.h"

class ClientContainer;
class Client;
class Workspace;

class WindowManager : public ActionSet
{
public:
    WindowManager(Workspace *workspace, std::string action_set_name) : ActionSet(action_set_name),
        _workspace(workspace) {}
    virtual ~WindowManager() {}

    virtual ClientContainer *activeClientContainer() = 0;
    virtual void manageClient(Client *client) = 0;
    virtual void layout() = 0;
    virtual void handleMaximizedChanged() = 0;

    Workspace *workspace() { return _workspace; }

private:
    Workspace *_workspace;
};

#endif
