#ifndef __WINDOW_MANAGER_SIMPLE_H__
#define __WINDOW_MANAGER_SIMPLE_H__

#include "window_manager.h"
#include "workspace.h"
#include "client.h"

class WindowManagerSimple : public WindowManager
{
public:
    WindowManagerSimple(Workspace *workspace, std::string action_set_name) :
        WindowManager(workspace, action_set_name) {}

    virtual Client *activeClient() { return 0; }
    virtual void manageClient(Client *client) override {
        client->setIsFloating(true);
        workspace()->addChild(client);
    }
    virtual void layout() override {}
    virtual void makeClientActive(Client *client) {}
};

#endif
