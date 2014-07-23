#ifndef __WINDOW_MANAGER_SIMPLE_H__
#define __WINDOW_MANAGER_SIMPLE_H__

#include <list>

#include "window_manager.h"
#include "workspace.h"
#include "client.h"
#include "client_frame.h"
#include "common.h"

class WindowManagerSimple : public WindowManager
{
public:
    WindowManagerSimple(Workspace *workspace, Mode *mode) :
        WindowManager(workspace, mode) {}

    ~WindowManagerSimple() {
        assert(_clients.empty());
    }

    virtual void setActive(bool active) override {
        WindowManager::setActive(active);
        for (ClientFrame *client : _clients)
            client->setMapped(active);
    }

    virtual ClientWrapper *activeClient() { return 0; }

    virtual void manageClient(ClientWrapper *client) override {
        ClientFrame *frame = new ClientFrame(client);
        workspace()->addChild(frame);
        frame->setHasDecoration(true);
        frame->setMapped(isActive());
        _clients.push_back(frame);
    }
    virtual void unmanageClient(ClientWrapper *client) override {
        for (std::list<ClientFrame*>::iterator it = _clients.begin();
                it != _clients.end();
                it++)
        {
            ClientFrame *frame = *it;
            if (frame->client() == client) {
                _clients.erase(it);
                delete frame;
                return;
            }
        }
    }

    virtual void makeClientActive(ClientWrapper *client) override {
        assert(0);
    }

    virtual void layout() override {}

protected:
    virtual void performAction(int id) override {}
    virtual void performComplexAction(const ComplexAction *action, const ComplexAction::Parameters *parameters) override {}

private:
    std::list<ClientFrame*> _clients;
};

#endif
