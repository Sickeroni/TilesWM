#ifndef __WINDOW_MANAGER_SIMPLE_H__
#define __WINDOW_MANAGER_SIMPLE_H__

#include <list>

#include "window_manager.h"
#include "client.h"
#include "client_frame.h"
#include "common.h"

class WindowManagerSimple : public WindowManager
{
public:
    WindowManagerSimple(Widget *parent_widget, Mode *mode) :
        WindowManager(parent_widget, mode) {}

    ~WindowManagerSimple() {
        assert(_clients.empty());
    }

    virtual void setActive(bool active) override {
        WindowManager::setActive(active);
        for (ClientFrame *client : _clients)
            client->setMapped(active);
    }

    virtual ClientWrapper *activeClient() {
        return _active_client ? _active_client->client() : 0;
    }

    virtual void manageClient(ClientWrapper *client) override {
        ClientFrame *frame = new ClientFrame(client);
        frame->reparent(parentWidget());
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
                if (_active_client == frame)
                    _active_client = 0;
                _clients.erase(it);
                delete frame;
                return;
            }
        }
    }

    virtual void makeClientActive(ClientWrapper *client) override {
        for (ClientFrame *frame: _clients) {
            if (frame->client() == client) {
                _active_client = frame;
                break;
            }
        }
    }

    virtual void layout() override {}

    virtual void redrawAll() override {
        for (ClientFrame *client : _clients)
            client->redraw();
    }

    virtual void setHasFocus(bool /*has_focus*/) override {
        //unused
    }

protected:
    virtual void performAction(int /*id*/) override {}
    virtual void performComplexAction(const ComplexAction */*action*/, const ComplexAction::Parameters */*parameters*/) override {}

private:
    std::list<ClientFrame*> _clients;
    ClientFrame *_active_client = 0;
};

#endif
