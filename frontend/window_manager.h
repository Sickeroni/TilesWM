#ifndef __WINDOW_MANAGER_H__
#define __WINDOW_MANAGER_H__

#include "key_grab_handler.h"
#include "mode.h"

#include <string>
#include <vector>
#include <list>

class Client;
class Workspace;
class Widget;
class Mode;
class KeyBindingSet;
class ClientWrapper;

class WindowManager : public KeyGrabHandler
{
public:
    WindowManager(Workspace *workspace, Mode *mode) :
        _workspace(workspace),
        _mode(mode) {}
    virtual ~WindowManager();

    virtual const KeyGrabSet *grabs() override { return _mode->keyBindings()->keyGrabs(); }

    virtual void layout() = 0;
    virtual void manageClient(ClientWrapper *client) = 0;
//     virtual void unmanageAllClients(std::vector<Client*> &unmanaged_clients) = 0;
    virtual ClientWrapper *activeClient() = 0;
    virtual void makeClientActive(ClientWrapper *client) = 0;


    // FIXME remove ?
    // and instead have makeClientActive(Client*) ?
    // or have both ?
//     virtual void makeContainerActive(Container *container) = 0;
    // this also returns true if container is a parent of the active container
    //FIXME better name or remove
    // alternative: container->isParentOf(activeContainer()) / activeContainer()->isChildIf(container)
//     virtual bool isContainerActive(Container *container) = 0;

    virtual void setActive(bool active);

    void manageClient(Client *client);
    void unmanageClient(Client *client);
    void makeClientActive(Client *client);

    Workspace *workspace() { return _workspace; }
    bool isActive() { return _is_active; }

protected:
    virtual const KeyBindingSet *keyBindings() override { return _mode->keyBindings(); }

private:
    Workspace *_workspace = 0;
    Mode *_mode = 0;
    bool _is_active = false;
    std::list<ClientWrapper*> _clients;
};

#endif
