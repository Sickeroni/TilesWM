#ifndef __WINDOW_MANAGER_SIMPLE_H__
#define __WINDOW_MANAGER_SIMPLE_H__

#include <list>

#include "window_manager.h"
#include "workspace.h"
#include "client.h"
#include "common.h"

class WindowManagerSimple : public WindowManager
{
public:
    WindowManagerSimple(Workspace *workspace, Mode *mode) :
        WindowManager(workspace, mode) {}

    ~WindowManagerSimple() {
        assert(_clients.empty());
    }

    virtual Client *activeClient() { return 0; }
    virtual void manageClient(Client *client) override {
        debug;
        _clients.push_back(client);
        client->setIsFloating(true);
        client->setHasDecoration(true);
        workspace()->addChild(client);
    }
    virtual void unmanageClient(Client *client) override {
        workspace()->removeChild(client);
        _clients.remove(client);
    }
    virtual void unmanageAllClients(std::vector<Client*> &unmanaged_clients) override {
        for (Client *client : _clients) {
            workspace()->removeChild(client);
            unmanaged_clients.push_back(client);
        }
        _clients.clear();
    }
    virtual void layout() override {}
    virtual void makeClientActive(Client *client) {}

protected:
    virtual void performAction(int id) override {}
    virtual void performComplexAction(const ComplexAction *action, const ComplexAction::Parameters *parameters) override {}

private:
    std::list<Client*> _clients;
};

#endif
