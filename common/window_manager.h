#ifndef __WINDOW_MANAGER_H__
#define __WINDOW_MANAGER_H__

#include "action_set.h"

class Client;
class Workspace;
class Widget;

class WindowManager : public ActionSet
{
public:
    WindowManager(Workspace *workspace, std::string action_set_name) : ActionSet(action_set_name),
        _workspace(workspace) {}
    virtual ~WindowManager() {}

    virtual Client *activeClient() = 0;
//     virtual Container *activeContainer() = 0;
    virtual void manageClient(Client *client) = 0;
    virtual void layout() = 0;
    virtual void handleMaximizedChanged() = 0;
    virtual void makeClientActive(Widget *client) = 0;
    // FIXME remove ?
    // and instead have makeClientActive(Client*) ?
    // or have both ?
//     virtual void makeContainerActive(Container *container) = 0;
    // this also returns true if container is a parent of the active container
    //FIXME better name or remove
    // alternative: container->isParentOf(activeContainer()) / activeContainer()->isChildIf(container)
//     virtual bool isContainerActive(Container *container) = 0;

    Workspace *workspace() { return _workspace; }

private:
    Workspace *_workspace;
};

#endif
